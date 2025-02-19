set(SOURCES
	${NCINE_ROOT}/src/base/Random.cpp
	${NCINE_ROOT}/src/base/Object.cpp
	${NCINE_ROOT}/src/base/HashFunctions.cpp
	${NCINE_ROOT}/src/base/CString.cpp
	${NCINE_ROOT}/src/base/Utf8.cpp
	${NCINE_ROOT}/src/base/String.cpp
	${NCINE_ROOT}/src/base/Clock.cpp
	${NCINE_ROOT}/src/ServiceLocator.cpp
	${NCINE_ROOT}/src/FileLogger.cpp
	${NCINE_ROOT}/src/ArrayIndexer.cpp
	${NCINE_ROOT}/src/TimeStamp.cpp
	${NCINE_ROOT}/src/Timer.cpp
	${NCINE_ROOT}/src/FrameTimer.cpp
	${NCINE_ROOT}/src/Font.cpp
	${NCINE_ROOT}/src/FntParser.cpp
	${NCINE_ROOT}/src/FontGlyph.cpp
	${NCINE_ROOT}/src/FileSystem.cpp
	${NCINE_ROOT}/src/IFile.cpp
	${NCINE_ROOT}/src/MemoryFile.cpp
	${NCINE_ROOT}/src/StandardFile.cpp
	${NCINE_ROOT}/src/input/IInputManager.cpp
	${NCINE_ROOT}/src/input/JoyMapping.cpp
	${NCINE_ROOT}/src/graphics/Color.cpp
	${NCINE_ROOT}/src/graphics/Colorf.cpp
	${NCINE_ROOT}/src/graphics/ColorHdr.cpp
	${NCINE_ROOT}/src/graphics/IGfxDevice.cpp
	${NCINE_ROOT}/src/graphics/GfxCapabilities.cpp
	${NCINE_ROOT}/src/graphics/RenderResources.cpp
	${NCINE_ROOT}/src/graphics/RenderCommand.cpp
	${NCINE_ROOT}/src/graphics/RenderQueue.cpp
	${NCINE_ROOT}/src/graphics/Material.cpp
	${NCINE_ROOT}/src/graphics/Geometry.cpp
	${NCINE_ROOT}/src/graphics/TextureFormat.cpp
	${NCINE_ROOT}/src/graphics/ITextureLoader.cpp
	${NCINE_ROOT}/src/graphics/TextureLoaderRaw.cpp
	${NCINE_ROOT}/src/graphics/TextureLoaderDds.cpp
	${NCINE_ROOT}/src/graphics/TextureLoaderPvr.cpp
	${NCINE_ROOT}/src/graphics/TextureLoaderKtx.cpp
	${NCINE_ROOT}/src/graphics/ITextureSaver.cpp
	${NCINE_ROOT}/src/graphics/Texture.cpp
	${NCINE_ROOT}/src/graphics/Shader.cpp
	${NCINE_ROOT}/src/graphics/ShaderState.cpp
	${NCINE_ROOT}/src/graphics/DrawableNode.cpp
	${NCINE_ROOT}/src/graphics/SceneNode.cpp
	${NCINE_ROOT}/src/graphics/BaseSprite.cpp
	${NCINE_ROOT}/src/graphics/Sprite.cpp
	${NCINE_ROOT}/src/graphics/MeshSprite.cpp
	${NCINE_ROOT}/src/Application.cpp
	${NCINE_ROOT}/src/AppConfiguration.cpp
	${NCINE_ROOT}/src/graphics/Particle.cpp
	${NCINE_ROOT}/src/graphics/ParticleAffectors.cpp
	${NCINE_ROOT}/src/graphics/ParticleSystem.cpp
	${NCINE_ROOT}/src/graphics/ParticleInitializer.cpp
	${NCINE_ROOT}/src/graphics/TextNode.cpp
	${NCINE_ROOT}/src/graphics/RectAnimation.cpp
	${NCINE_ROOT}/src/graphics/AnimatedSprite.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLBufferObject.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLFramebufferObject.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLRenderbuffer.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLShader.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLShaderProgram.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLShaderUniforms.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLUniform.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLUniformCache.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLAttribute.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLTexture.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLVertexArrayObject.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLUniformBlock.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLUniformBlockCache.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLShaderUniformBlocks.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLScissorTest.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLDepthTest.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLBlending.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLCullFace.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLClearColor.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLViewport.cpp
	${NCINE_ROOT}/src/graphics/RenderBuffersManager.cpp
	${NCINE_ROOT}/src/graphics/RenderBatcher.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLDebug.cpp
	${NCINE_ROOT}/src/graphics/RenderStatistics.cpp
	${NCINE_ROOT}/src/graphics/opengl/GLVertexFormat.cpp
	${NCINE_ROOT}/src/graphics/RenderVaoPool.cpp
	${NCINE_ROOT}/src/graphics/RenderCommandPool.cpp
	${NCINE_ROOT}/src/graphics/Viewport.cpp
	${NCINE_ROOT}/src/graphics/ScreenViewport.cpp
	${NCINE_ROOT}/src/graphics/Camera.cpp
)
