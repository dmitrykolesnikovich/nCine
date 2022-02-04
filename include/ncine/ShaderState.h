#ifndef CLASS_NCINE_SHADERSTATE
#define CLASS_NCINE_SHADERSTATE

#include "common_defines.h"

namespace ncine {

class DrawableNode;
class Shader;

/// Shader state class for the user to use custom shaders
class DLL_PUBLIC ShaderState
{
  public:
	using UpdateFunctionT = void (*)(DrawableNode &node);

	explicit ShaderState(DrawableNode &node);

	inline const Shader *shader() const { return shader_; }
	bool setShader(Shader *shader);

	bool setAttribute(const char *name, int stride, unsigned long int pointer);

	bool setUniformInt(const char *blockName, const char *name, const int *vector);
	bool setUniformInt(const char *blockName, const char *name, int value0);
	bool setUniformInt(const char *blockName, const char *name, int value0, int value1);
	bool setUniformInt(const char *blockName, const char *name, int value0, int value1, int value2);
	bool setUniformInt(const char *blockName, const char *name, int value0, int value1, int value2, int value3);

	bool setUniformFloat(const char *blockName, const char *name, const float *vector);
	bool setUniformFloat(const char *blockName, const char *name, float value0);
	bool setUniformFloat(const char *blockName, const char *name, float value0, float value1);
	bool setUniformFloat(const char *blockName, const char *name, float value0, float value1, float value2);
	bool setUniformFloat(const char *blockName, const char *name, float value0, float value1, float value2, float value3);

	inline UpdateFunctionT updateFunction() { return updateFunction_; }
	inline void setUpdateFunction(UpdateFunctionT updateFunction) { updateFunction_ = updateFunction; }

	inline void *userData() { return userData_; }
	inline void setUserData(void *userData) { userData_ = userData; }

  private:
	DrawableNode *node_;
	Shader *shader_;
	int previousShaderType_;

	UpdateFunctionT updateFunction_;
	void *userData_;
};

}

#endif
