#ifndef CLASS_MYEVENTHANDLER
#define CLASS_MYEVENTHANDLER

#include <ncine/IAppEventHandler.h>
#include <ncine/IInputEventHandler.h>
#include <nctl/StaticArray.h>

namespace nctl {

class String;

}

namespace ncine {

class AppConfiguration;
class Viewport;
class SceneNode;
class Texture;
class Font;
class TextNode;
class Shader;
class ShaderState;
class Sprite;
class MeshSprite;

}

namespace nc = ncine;

/// My nCine event handler
class MyEventHandler :
    public nc::IAppEventHandler,
    public nc::IInputEventHandler
{
  public:
	static const unsigned int NumTextures = 4;
	static const unsigned int NumSprites = 8;

	void onPreInit(nc::AppConfiguration &config) override;
	void onInit() override;
	void onFrameStart() override;
	void onPostUpdate() override;
	void onDrawViewport(nc::Viewport &viewport) override;

#ifdef __ANDROID__
	void onTouchDown(const nc::TouchEvent &event) override;
	void onTouchMove(const nc::TouchEvent &event) override;
#endif
	void onKeyReleased(const nc::KeyboardEvent &event) override;
	void onMouseButtonPressed(const nc::MouseEvent &event) override;
	void onMouseMoved(const nc::MouseState &state) override;

  private:
	float angle_;
	int numBlurPasses_;
	int currentViewportSetup_;

	bool withAtlas_;
	nctl::UniquePtr<nc::Texture> texture0_;
	nctl::UniquePtr<nc::Texture> texture1_;
	nctl::UniquePtr<nc::Texture> bloomTexture_;
	nctl::UniquePtr<nc::Viewport> sceneViewport_;
	nctl::UniquePtr<nc::Viewport> sceneViewportMrt_;
	nctl::UniquePtr<nc::Viewport> pingViewport_;
	nctl::UniquePtr<nc::Viewport> pongViewport_;
	nctl::UniquePtr<nc::SceneNode> rootNode_;
	nctl::UniquePtr<nc::SceneNode> rootNodeMrt_;
	nctl::UniquePtr<nc::Sprite> vpSprite0_;
	nctl::UniquePtr<nc::Sprite> vpSprite1_;
	nctl::UniquePtr<nc::Sprite> sceneSprite_;
	nctl::UniquePtr<nc::Shader> vpBlurShader_;
	nctl::UniquePtr<nc::ShaderState> vpShaderStatePass0_;
	nctl::UniquePtr<nc::ShaderState> vpShaderStatePass1_;

	nctl::UniquePtr<nc::Sprite> blendingSprite_;
	nctl::UniquePtr<nc::Shader> blendingShader_;
	nctl::UniquePtr<nc::ShaderState> blendingShaderState_;

	nctl::StaticArray<nctl::UniquePtr<nc::Texture>, NumTextures> textures_;
	nctl::UniquePtr<nc::Texture> megaTexture_;
	nctl::UniquePtr<nc::Font> font_;
	nctl::UniquePtr<nctl::String> debugString_;
	nctl::UniquePtr<nc::TextNode> debugText_;

	nctl::UniquePtr<nc::Shader> spriteShader_;
	nctl::UniquePtr<nc::Shader> spriteMrtShader_;
	nctl::UniquePtr<nc::Shader> batchedSpriteShader_;
	nctl::UniquePtr<nc::Shader> batchedSpriteMrtShader_;
	nctl::StaticArray<nctl::UniquePtr<nc::Sprite>, NumSprites> sprites_;
	nctl::StaticArray<nctl::UniquePtr<nc::ShaderState>, NumSprites> spriteShaderStates_;

	nctl::UniquePtr<nc::Viewport> blendingViewport_;
	nctl::UniquePtr<nc::Shader> meshShader_;
	nctl::UniquePtr<nc::Shader> batchedMeshShader_;
	nctl::StaticArray<nctl::UniquePtr<nc::MeshSprite>, NumSprites> meshSprites_;
	nctl::StaticArray<nctl::UniquePtr<nc::ShaderState>, NumSprites> meshSpriteShaderStates_;

	nctl::UniquePtr<nc::Texture> diffuseTexture_;
	nctl::UniquePtr<nc::Texture> normalTexture_;
	nctl::UniquePtr<nc::Shader> multitextureShader_;
	nctl::UniquePtr<nc::Shader> batchedMultitextureShader_;
	nctl::StaticArray<nctl::UniquePtr<nc::Sprite>, NumSprites> multitextureSprites_;
	nctl::StaticArray<nctl::UniquePtr<nc::ShaderState>, NumSprites> multitextureShaderStates_;

	void setupAtlas();
	void setupTextures();
	void setupViewport();
	void setupMeshVertices(bool defaultShader);
	void checkClick(float x, float y);
};

#endif
