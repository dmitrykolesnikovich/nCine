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

ShaderState::ShaderState(DrawableNode &node)
    : node_(&node), shader_(nullptr),
      previousShaderType_(static_cast<int>(Material::ShaderProgramType::CUSTOM)),
      updateFunction_(nullptr), userData_(nullptr)
{
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

bool ShaderState::setShader(Shader *shader)
{
	bool shaderHasChanged = false;

	if (shader != shader_)
	{
		Material &material = node_->renderCommand_->material();
		if (shader == nullptr)
		{
			const Material::ShaderProgramType programType = static_cast<Material::ShaderProgramType>(previousShaderType_);
			material.setShaderProgramType(programType);
			setUpdateFunction(nullptr);
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

bool ShaderState::setAttribute(const char *name, int stride, unsigned long int pointer)
{
	if (shader_ == nullptr || name == nullptr)
		return false;

	Material &material = node_->renderCommand_->material();
	if (material.attribute(name))
	{
		material.attribute(name)->setVboParameters(stride, reinterpret_cast<void *>(pointer));
		return true;
	}

	return false;
}

bool ShaderState::setUniformInt(const char *blockName, const char *name, const int *vector)
{
	if (shader_ == nullptr || name == nullptr || vector == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setIntVector(vector);

	return (uniform != nullptr);
}

bool ShaderState::setUniformInt(const char *blockName, const char *name, int value0)
{
	if (shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setIntValue(value0);

	return (uniform != nullptr);
}

bool ShaderState::setUniformInt(const char *blockName, const char *name, int value0, int value1)
{
	if (shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setIntValue(value0, value1);

	return (uniform != nullptr);
}

bool ShaderState::setUniformInt(const char *blockName, const char *name, int value0, int value1, int value2)
{
	if (shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setIntValue(value0, value1, value2);

	return (uniform != nullptr);
}

bool ShaderState::setUniformInt(const char *blockName, const char *name, int value0, int value1, int value2, int value3)
{
	if (shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setIntValue(value0, value1, value2, value3);

	return (uniform != nullptr);
}

bool ShaderState::setUniformFloat(const char *blockName, const char *name, const float *vector)
{
	if (shader_ == nullptr || name == nullptr || vector == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setFloatVector(vector);

	return (uniform != nullptr);
}

bool ShaderState::setUniformFloat(const char *blockName, const char *name, float value0)
{
	if (shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setFloatValue(value0);

	return (uniform != nullptr);
}

bool ShaderState::setUniformFloat(const char *blockName, const char *name, float value0, float value1)
{
	if (shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setFloatValue(value0, value1);

	return (uniform != nullptr);
}

bool ShaderState::setUniformFloat(const char *blockName, const char *name, float value0, float value1, float value2)
{
	if (shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setFloatValue(value0, value1, value2);

	return (uniform != nullptr);
}

bool ShaderState::setUniformFloat(const char *blockName, const char *name, float value0, float value1, float value2, float value3)
{
	if (shader_ == nullptr || name == nullptr)
		return false;

	GLUniformCache *uniform = retrieveUniform(node_->renderCommand_->material(), blockName, name);
	if (uniform)
		uniform->setFloatValue(value0, value1, value2, value3);

	return (uniform != nullptr);
}

}
