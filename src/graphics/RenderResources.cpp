#include <cstddef> // for `offsetof()`
#include <nctl/HashMapIterator.h>
#include "RenderResources.h"
#include "RenderBuffersManager.h"
#include "RenderVaoPool.h"
#include "RenderCommandPool.h"
#include "RenderBatcher.h"
#include "Camera.h"
#include "Application.h"

#ifdef WITH_EMBEDDED_SHADERS
	#include "shader_strings.h"
#else
	#include "FileSystem.h" // for dataPath()
#endif

namespace ncine {

///////////////////////////////////////////////////////////
// STATIC DEFINITIONS
///////////////////////////////////////////////////////////

nctl::UniquePtr<RenderBuffersManager> RenderResources::buffersManager_;
nctl::UniquePtr<RenderVaoPool> RenderResources::vaoPool_;
nctl::UniquePtr<RenderCommandPool> RenderResources::renderCommandPool_;
nctl::UniquePtr<RenderBatcher> RenderResources::renderBatcher_;

nctl::UniquePtr<GLShaderProgram> RenderResources::defaultShaderPrograms_[NumDefaultShaderPrograms];
nctl::HashMap<const GLShaderProgram *, GLShaderProgram *> RenderResources::batchedShaders_(32);

unsigned char RenderResources::cameraUniformsBuffer_[UniformsBufferSize];
nctl::HashMap<GLShaderProgram *, RenderResources::CameraUniformData> RenderResources::cameraUniformDataMap_(32);

Camera *RenderResources::currentCamera_ = nullptr;
nctl::UniquePtr<Camera> RenderResources::defaultCamera_;
Viewport *RenderResources::currentViewport_ = nullptr;

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

GLShaderProgram *RenderResources::shaderProgram(Material::ShaderProgramType shaderProgramType)
{
	if (shaderProgramType != Material::ShaderProgramType::CUSTOM)
		return defaultShaderPrograms_[static_cast<int>(shaderProgramType)].get();
	else
		return nullptr;
}

GLShaderProgram *RenderResources::batchedShader(const GLShaderProgram *shader)
{
	GLShaderProgram *batchedShader = nullptr;

	GLShaderProgram **findResult = batchedShaders_.find(shader);
	if (findResult != nullptr)
		batchedShader = *findResult;

	return batchedShader;
}

bool RenderResources::registerBatchedShader(const GLShaderProgram *shader, GLShaderProgram *batchedShader)
{
	FATAL_ASSERT(shader != nullptr);
	FATAL_ASSERT(batchedShader != nullptr);
	FATAL_ASSERT(shader != batchedShader);

	if (batchedShaders_.loadFactor() >= 0.8f)
		batchedShaders_.rehash(batchedShaders_.capacity() * 2);
	const bool inserted = batchedShaders_.insert(shader, batchedShader);

	return inserted;
}

bool RenderResources::unregisterBatchedShader(const GLShaderProgram *shader)
{
	ASSERT(shader != nullptr);
	const bool removed = batchedShaders_.remove(shader);
	return removed;
}

RenderResources::CameraUniformData *RenderResources::findCameraUniformData(GLShaderProgram *shaderProgram)
{
	return cameraUniformDataMap_.find(shaderProgram);
}

void RenderResources::insertCameraUniformData(GLShaderProgram *shaderProgram, CameraUniformData &&cameraUniformData)
{
	FATAL_ASSERT(shaderProgram != nullptr);

	if (cameraUniformDataMap_.loadFactor() >= 0.8f)
		cameraUniformDataMap_.rehash(cameraUniformDataMap_.capacity() * 2);

	cameraUniformDataMap_.insert(shaderProgram, nctl::move(cameraUniformData));
}

bool RenderResources::removeCameraUniformData(GLShaderProgram *shaderProgram)
{
	bool hasRemoved = false;

	if (cameraUniformDataMap_.isEmpty() == false)
		hasRemoved = cameraUniformDataMap_.remove(shaderProgram);

	return hasRemoved;
}

void RenderResources::setDefaultAttributesParameters(GLShaderProgram &shaderProgram)
{
	if (shaderProgram.numAttributes() > 0)
	{
		GLVertexFormat::Attribute *positionAttribute = shaderProgram.attribute(Material::PositionAttributeName);
		GLVertexFormat::Attribute *texCoordsAttribute = shaderProgram.attribute(Material::TexCoordsAttributeName);
		GLVertexFormat::Attribute *meshIndexAttribute = shaderProgram.attribute(Material::MeshIndexAttributeName);

		// The stride check avoid overwriting VBO parameters for custom mesh shaders attributes
		if (positionAttribute != nullptr && texCoordsAttribute != nullptr && meshIndexAttribute != nullptr)
		{
			if (positionAttribute->stride() == 0)
				positionAttribute->setVboParameters(sizeof(VertexFormatPos2Tex2Index), reinterpret_cast<void *>(offsetof(VertexFormatPos2Tex2Index, position)));

			if (texCoordsAttribute->stride() == 0)
				texCoordsAttribute->setVboParameters(sizeof(VertexFormatPos2Tex2Index), reinterpret_cast<void *>(offsetof(VertexFormatPos2Tex2Index, texcoords)));

			if (meshIndexAttribute->stride() == 0)
				meshIndexAttribute->setVboParameters(sizeof(VertexFormatPos2Tex2Index), reinterpret_cast<void *>(offsetof(VertexFormatPos2Tex2Index, drawindex)));
		}
		else if (positionAttribute != nullptr && texCoordsAttribute == nullptr && meshIndexAttribute != nullptr)
		{
			if (positionAttribute->stride() == 0)
				positionAttribute->setVboParameters(sizeof(VertexFormatPos2Index), reinterpret_cast<void *>(offsetof(VertexFormatPos2Index, position)));

			if (meshIndexAttribute->stride() == 0)
				meshIndexAttribute->setVboParameters(sizeof(VertexFormatPos2Index), reinterpret_cast<void *>(offsetof(VertexFormatPos2Index, drawindex)));
		}
		else if (positionAttribute != nullptr && texCoordsAttribute != nullptr && meshIndexAttribute == nullptr)
		{
			if (positionAttribute->stride() == 0)
				positionAttribute->setVboParameters(sizeof(VertexFormatPos2Tex2), reinterpret_cast<void *>(offsetof(VertexFormatPos2Tex2, position)));

			if (texCoordsAttribute->stride() == 0)
				texCoordsAttribute->setVboParameters(sizeof(VertexFormatPos2Tex2), reinterpret_cast<void *>(offsetof(VertexFormatPos2Tex2, texcoords)));
		}
		else if (positionAttribute != nullptr && texCoordsAttribute == nullptr && meshIndexAttribute == nullptr)
		{
			if (positionAttribute->stride() == 0)
				positionAttribute->setVboParameters(sizeof(VertexFormatPos2), reinterpret_cast<void *>(offsetof(VertexFormatPos2, position)));
		}
	}
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

namespace {

	struct ShaderLoad
	{
		nctl::UniquePtr<GLShaderProgram> &shaderProgram;
		const char *vertexShader;
		const char *fragmentShader;
		GLShaderProgram::Introspection introspection;
		const char *objectLabel;
	};

}

void RenderResources::setCurrentCamera(Camera *camera)
{
	if (camera != nullptr)
		currentCamera_ = camera;
	else
		currentCamera_ = defaultCamera_.get();
}

void RenderResources::updateCameraUniforms()
{
	// The buffer is shared among every shader program. There is no need to call `setFloatVector()` as `setDirty()` is enough.
	memcpy(cameraUniformsBuffer_, currentCamera_->projection().data(), 64);
	memcpy(cameraUniformsBuffer_ + 64, currentCamera_->view().data(), 64);
	for (nctl::HashMap<GLShaderProgram *, CameraUniformData>::Iterator i = cameraUniformDataMap_.begin(); i != cameraUniformDataMap_.end(); ++i)
	{
		CameraUniformData &cameraUniformData = *i;

		if (cameraUniformData.camera != currentCamera_)
			(*i).shaderUniforms.setDirty(true);
		else
		{
			if (cameraUniformData.updateFrameProjectionMatrix < currentCamera_->updateFrameProjectionMatrix())
				(*i).shaderUniforms.uniform(Material::ProjectionMatrixUniformName)->setDirty(true);
			if (cameraUniformData.updateFrameViewMatrix < currentCamera_->updateFrameViewMatrix())
				(*i).shaderUniforms.uniform(Material::ViewMatrixUniformName)->setDirty(true);
		}

		cameraUniformData.camera = currentCamera_;
		cameraUniformData.updateFrameProjectionMatrix = currentCamera_->updateFrameProjectionMatrix();
		cameraUniformData.updateFrameViewMatrix = currentCamera_->updateFrameViewMatrix();
	}
}

void RenderResources::setCurrentViewport(Viewport *viewport)
{
	FATAL_ASSERT(viewport != nullptr);
	currentViewport_ = viewport;
}

void RenderResources::create()
{
	LOGI("Creating rendering resources...");

	const AppConfiguration &appCfg = theApplication().appConfiguration();
	buffersManager_ = nctl::makeUnique<RenderBuffersManager>(appCfg.useBufferMapping, appCfg.vboSize, appCfg.iboSize);
	vaoPool_ = nctl::makeUnique<RenderVaoPool>(appCfg.vaoPoolSize);
	renderCommandPool_ = nctl::makeUnique<RenderCommandPool>(appCfg.vaoPoolSize);
	renderBatcher_ = nctl::makeUnique<RenderBatcher>();
	defaultCamera_ = nctl::makeUnique<Camera>();
	currentCamera_ = defaultCamera_.get();

	ShaderLoad shadersToLoad[] = {
#ifndef WITH_EMBEDDED_SHADERS
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::SPRITE)], "sprite_vs.glsl", "sprite_fs.glsl", GLShaderProgram::Introspection::ENABLED, "Sprite" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::SPRITE_GRAY)], "sprite_vs.glsl", "sprite_gray_fs.glsl", GLShaderProgram::Introspection::ENABLED, "Sprite_Gray" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::SPRITE_NO_TEXTURE)], "sprite_notexture_vs.glsl", "sprite_notexture_fs.glsl", GLShaderProgram::Introspection::ENABLED, "Sprite_NoTexture" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::MESH_SPRITE)], "meshsprite_vs.glsl", "sprite_fs.glsl", GLShaderProgram::Introspection::ENABLED, "MeshSprite" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::MESH_SPRITE_GRAY)], "meshsprite_vs.glsl", "sprite_gray_fs.glsl", GLShaderProgram::Introspection::ENABLED, "MeshSprite_Gray" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::MESH_SPRITE_NO_TEXTURE)], "meshsprite_notexture_vs.glsl", "sprite_notexture_fs.glsl", GLShaderProgram::Introspection::ENABLED, "MeshSprite_NoTexture" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::TEXTNODE_ALPHA)], "textnode_vs.glsl", "textnode_alpha_fs.glsl", GLShaderProgram::Introspection::ENABLED, "TextNode_Alpha" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::TEXTNODE_RED)], "textnode_vs.glsl", "textnode_red_fs.glsl", GLShaderProgram::Introspection::ENABLED, "TextNode_Red" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::TEXTNODE_SPRITE)], "textnode_vs.glsl", "sprite_fs.glsl", GLShaderProgram::Introspection::ENABLED, "TextNode_Sprite" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_SPRITES)], "batched_sprites_vs.glsl", "sprite_fs.glsl", GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_Sprites" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_SPRITES_GRAY)], "batched_sprites_vs.glsl", "sprite_gray_fs.glsl", GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_Sprites_Gray" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_SPRITES_NO_TEXTURE)], "batched_sprites_notexture_vs.glsl", "sprite_notexture_fs.glsl", GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_Sprites_NoTexture" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_MESH_SPRITES)], "batched_meshsprites_vs.glsl", "sprite_fs.glsl", GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_MeshSprites" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_MESH_SPRITES_GRAY)], "batched_meshsprites_vs.glsl", "sprite_gray_fs.glsl", GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_MeshSprites_Gray" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_MESH_SPRITES_NO_TEXTURE)], "batched_meshsprites_notexture_vs.glsl", "sprite_notexture_fs.glsl", GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_MeshSprites_NoTexture" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_TEXTNODES_ALPHA)], "batched_textnodes_vs.glsl", "textnode_alpha_fs.glsl", GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_TextNodes_Alpha" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_TEXTNODES_RED)], "batched_textnodes_vs.glsl", "textnode_red_fs.glsl", GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_TextNodes_Red" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_TEXTNODES_SPRITE)], "batched_textnodes_vs.glsl", "sprite_fs.glsl", GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_TextNodes_Sprite" }
#else
		// Skipping the initial new line character of the raw string literal
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::SPRITE)], ShaderStrings::sprite_vs + 1, ShaderStrings::sprite_fs + 1, GLShaderProgram::Introspection::ENABLED, "Sprite" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::SPRITE_GRAY)], ShaderStrings::sprite_vs + 1, ShaderStrings::sprite_gray_fs + 1, GLShaderProgram::Introspection::ENABLED, "Sprite_Gray" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::SPRITE_NO_TEXTURE)], ShaderStrings::sprite_notexture_vs + 1, ShaderStrings::sprite_notexture_fs + 1, GLShaderProgram::Introspection::ENABLED, "Sprite_NoTexture" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::MESH_SPRITE)], ShaderStrings::meshsprite_vs + 1, ShaderStrings::sprite_fs + 1, GLShaderProgram::Introspection::ENABLED, "MeshSprite" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::MESH_SPRITE_GRAY)], ShaderStrings::meshsprite_vs + 1, ShaderStrings::sprite_gray_fs + 1, GLShaderProgram::Introspection::ENABLED, "MeshSprite_Gray" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::MESH_SPRITE_NO_TEXTURE)], ShaderStrings::meshsprite_notexture_vs + 1, ShaderStrings::sprite_notexture_fs + 1, GLShaderProgram::Introspection::ENABLED, "MeshSprite_NoTexture" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::TEXTNODE_ALPHA)], ShaderStrings::textnode_vs + 1, ShaderStrings::textnode_alpha_fs + 1, GLShaderProgram::Introspection::ENABLED, "TextNode_Alpha" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::TEXTNODE_RED)], ShaderStrings::textnode_vs + 1, ShaderStrings::textnode_red_fs + 1, GLShaderProgram::Introspection::ENABLED, "TextNode_Red" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::TEXTNODE_SPRITE)], ShaderStrings::textnode_vs + 1, ShaderStrings::sprite_fs + 1, GLShaderProgram::Introspection::ENABLED, "TextNode_Sprite" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_SPRITES)], ShaderStrings::batched_sprites_vs + 1, ShaderStrings::sprite_fs + 1, GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_Sprites" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_SPRITES_GRAY)], ShaderStrings::batched_sprites_vs + 1, ShaderStrings::sprite_gray_fs + 1, GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_Sprites_Gray" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_SPRITES_NO_TEXTURE)], ShaderStrings::batched_sprites_notexture_vs + 1, ShaderStrings::sprite_notexture_fs + 1, GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_Sprites_NoTexture" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_MESH_SPRITES)], ShaderStrings::batched_meshsprites_vs + 1, ShaderStrings::sprite_fs + 1, GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_MeshSprites" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_MESH_SPRITES_GRAY)], ShaderStrings::batched_meshsprites_vs + 1, ShaderStrings::sprite_gray_fs + 1, GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_MeshSprites_Gray" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_MESH_SPRITES_NO_TEXTURE)], ShaderStrings::batched_meshsprites_notexture_vs + 1, ShaderStrings::sprite_notexture_fs + 1, GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_MeshSprites_NoTexture" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_TEXTNODES_ALPHA)], ShaderStrings::batched_textnodes_vs + 1, ShaderStrings::textnode_alpha_fs + 1, GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_TextNodes_Alpha" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_TEXTNODES_RED)], ShaderStrings::batched_textnodes_vs + 1, ShaderStrings::textnode_red_fs + 1, GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_TextNodes_Red" },
		{ RenderResources::defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_TEXTNODES_SPRITE)], ShaderStrings::batched_textnodes_vs + 1, ShaderStrings::sprite_fs + 1, GLShaderProgram::Introspection::NO_UNIFORMS_IN_BLOCKS, "Batched_TextNodes_Sprite" }
#endif
	};

	const GLShaderProgram::QueryPhase queryPhase = appCfg.deferShaderQueries ? GLShaderProgram::QueryPhase::DEFERRED : GLShaderProgram::QueryPhase::IMMEDIATE;
	const unsigned int numShaderToLoad = (sizeof(shadersToLoad) / sizeof(*shadersToLoad));
	FATAL_ASSERT(numShaderToLoad <= NumDefaultShaderPrograms);
	for (unsigned int i = 0; i < numShaderToLoad; i++)
	{
		const ShaderLoad &shaderToLoad = shadersToLoad[i];

		shaderToLoad.shaderProgram = nctl::makeUnique<GLShaderProgram>(queryPhase);
#ifndef WITH_EMBEDDED_SHADERS
		shaderToLoad.shaderProgram->attachShader(GL_VERTEX_SHADER, (fs::dataPath() + "shaders/" + shaderToLoad.vertexShader).data());
		shaderToLoad.shaderProgram->attachShader(GL_FRAGMENT_SHADER, (fs::dataPath() + "shaders/" + shaderToLoad.fragmentShader).data());
#else
		shaderToLoad.shaderProgram->attachShaderFromString(GL_VERTEX_SHADER, shaderToLoad.vertexShader);
		shaderToLoad.shaderProgram->attachShaderFromString(GL_FRAGMENT_SHADER, shaderToLoad.fragmentShader);
#endif
		shaderToLoad.shaderProgram->setObjectLabel(shaderToLoad.objectLabel);
		const bool hasLinked = shaderToLoad.shaderProgram->link(shaderToLoad.introspection);
		FATAL_ASSERT(hasLinked == true);
	}

	registerDefaultBatchedShaders();

	// Calculating a default projection matrix for all shader programs
	const float width = theApplication().width();
	const float height = theApplication().height();
	defaultCamera_->setOrthoProjection(0.0f, width, 0.0f, height);

	LOGI("Rendering resources created");
}

void RenderResources::createMinimal()
{
	LOGI("Creating a minimal set of rendering resources...");

	const AppConfiguration &appCfg = theApplication().appConfiguration();
	buffersManager_ = nctl::makeUnique<RenderBuffersManager>(appCfg.useBufferMapping, appCfg.vboSize, appCfg.iboSize);
	vaoPool_ = nctl::makeUnique<RenderVaoPool>(appCfg.vaoPoolSize);

	LOGI("Minimal rendering resources created");
}

void RenderResources::dispose()
{
	for (nctl::UniquePtr<GLShaderProgram> &shaderProgram : defaultShaderPrograms_)
		shaderProgram.reset(nullptr);

	ASSERT(cameraUniformDataMap_.isEmpty());

	defaultCamera_.reset(nullptr);
	renderBatcher_.reset(nullptr);
	renderCommandPool_.reset(nullptr);
	vaoPool_.reset(nullptr);
	buffersManager_.reset(nullptr);

	LOGI("Rendering resources disposed");
}

void RenderResources::registerDefaultBatchedShaders()
{
	batchedShaders_.insert(defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::SPRITE)].get(), defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_SPRITES)].get());
	batchedShaders_.insert(defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::SPRITE_GRAY)].get(), defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_SPRITES_GRAY)].get());
	batchedShaders_.insert(defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::SPRITE_NO_TEXTURE)].get(), defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_SPRITES_NO_TEXTURE)].get());
	batchedShaders_.insert(defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::MESH_SPRITE)].get(), defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_MESH_SPRITES)].get());
	batchedShaders_.insert(defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::MESH_SPRITE_GRAY)].get(), defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_MESH_SPRITES_GRAY)].get());
	batchedShaders_.insert(defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::MESH_SPRITE_NO_TEXTURE)].get(), defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_MESH_SPRITES_NO_TEXTURE)].get());
	batchedShaders_.insert(defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::TEXTNODE_ALPHA)].get(), defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_TEXTNODES_ALPHA)].get());
	batchedShaders_.insert(defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::TEXTNODE_RED)].get(), defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_TEXTNODES_RED)].get());
	batchedShaders_.insert(defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::TEXTNODE_SPRITE)].get(), defaultShaderPrograms_[static_cast<int>(Material::ShaderProgramType::BATCHED_TEXTNODES_SPRITE)].get());
}

}
