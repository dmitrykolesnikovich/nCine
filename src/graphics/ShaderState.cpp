#include <nctl/CString.h>
#include "ShaderState.h"
#include "Shader.h"
#include "DrawableNode.h"
#include "RenderCommand.h"
#include "Material.h"

namespace ncine {

namespace {

	GLUniformCache *retrieveUniform(Material &material, const char *blockName, const char *name)
	{
		GLUniformCache *uniform = nullptr;
		if (blockName)
		{
			GLUniformBlockCache *uniformBlock = material.uniformBlock(blockName);
			if (uniformBlock)
				uniform = uniformBlock->uniform(name);
		}
		else
			uniform = material.uniform(name);

		return uniform;
	}

}

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

ShaderState::ShaderState()
    : ShaderState(nullptr, nullptr)
{
}

ShaderState::ShaderState(DrawableNode *node, Shader *shader)
    : node_(nullptr), shader_(nullptr),
      previousShaderType_(static_cast<int>(Material::ShaderProgramType::CUSTOM))
{
	setNode(node);
	setShader(shader);
}

ShaderState::~ShaderState()
{
	setNode(nullptr);
	setShader(nullptr);
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

bool ShaderState::setNode(DrawableNode *node)
{
	bool nodeHasChanged = false;

	if (node != node_)
	{
		if (node_ != nullptr)
		{
			Material &prevMaterial = node_->renderCommand_->material();
			const Material::ShaderProgramType programType = static_cast<Material::ShaderProgramType>(previousShaderType_);
			prevMaterial.setShaderProgramType(programType);
		}

		if (node != nullptr)
		{
			Material &material = node->renderCommand_->material();
			previousShaderType_ = static_cast<int>(material.shaderProgramType());
		}
		node_ = node;

		if (shader_ != nullptr)
			setShader(shader_);

		nodeHasChanged = true;
	}

	return nodeHasChanged;
}

bool ShaderState::setShader(Shader *shader)
{
	bool shaderHasChanged = false;

	// Allow shader self-assignment to take into account the case where it loads new data
	if (node_ != nullptr)
	{
		Material &material = node_->renderCommand_->material();
		if (shader == nullptr)
		{
			const Material::ShaderProgramType programType = static_cast<Material::ShaderProgramType>(previousShaderType_);
			material.setShaderProgramType(programType);
		}
		else if (shader->isLinked())
		{
			if (material.shaderProgramType() != Material::ShaderProgramType::CUSTOM)
				previousShaderType_ = static_cast<int>(material.shaderProgramType());

			material.setShaderProgram(shader->glShaderProgram_.get());
		}

		shader_ = shader;
		node_->shaderHasChanged();
		shaderHasChanged = true;
	}

	return shaderHasChanged;
}

/*! \note Use this method when the content of the currently assigned shader changes */
bool ShaderState::resetShader()
{
	if (shader_ != nullptr && shader_->isLinked() && node_)
	{
		Material &material = node_->renderCommand_->material();
		material.setShaderProgram(shader_->glShaderProgram_.get());
		node_->shaderHasChanged();
		return true;
	}
	return false;
}

bool ShaderState::setTexture(unsigned int unit, const Texture *texture)
{
	if (node_ == nullptr)
		return false;

	Material &material = node_->renderCommand_->material();
	const bool result = texture ? material.setTexture(unit, *texture) : material.setTexture(unit, nullptr);

	return result;
}

bool ShaderState::setUniformInt(const char *blockName, const char *name, const int *vector)
{
	if (node_ == nullptr || shader_ == nullptr || name == nullptr || vector == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setIntVector(vector);

	return (uniform != nullptr);
}

bool ShaderState::setUniformInt(const char *blockName, const char *name, int value0)
{
	if (node_ == nullptr || shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setIntValue(value0);

	return (uniform != nullptr);
}

bool ShaderState::setUniformInt(const char *blockName, const char *name, int value0, int value1)
{
	if (node_ == nullptr || shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setIntValue(value0, value1);

	return (uniform != nullptr);
}

bool ShaderState::setUniformInt(const char *blockName, const char *name, int value0, int value1, int value2)
{
	if (node_ == nullptr || shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setIntValue(value0, value1, value2);

	return (uniform != nullptr);
}

bool ShaderState::setUniformInt(const char *blockName, const char *name, int value0, int value1, int value2, int value3)
{
	if (node_ == nullptr || shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setIntValue(value0, value1, value2, value3);

	return (uniform != nullptr);
}

bool ShaderState::setUniformFloat(const char *blockName, const char *name, const float *vector)
{
	if (node_ == nullptr || shader_ == nullptr || name == nullptr || vector == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setFloatVector(vector);

	return (uniform != nullptr);
}

bool ShaderState::setUniformFloat(const char *blockName, const char *name, float value0)
{
	if (node_ == nullptr || shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setFloatValue(value0);

	return (uniform != nullptr);
}

bool ShaderState::setUniformFloat(const char *blockName, const char *name, float value0, float value1)
{
	if (node_ == nullptr || shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setFloatValue(value0, value1);

	return (uniform != nullptr);
}

bool ShaderState::setUniformFloat(const char *blockName, const char *name, float value0, float value1, float value2)
{
	if (node_ == nullptr || shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setFloatValue(value0, value1, value2);

	return (uniform != nullptr);
}

bool ShaderState::setUniformFloat(const char *blockName, const char *name, float value0, float value1, float value2, float value3)
{
	if (node_ == nullptr || shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setFloatValue(value0, value1, value2, value3);

	return (uniform != nullptr);
}

}
