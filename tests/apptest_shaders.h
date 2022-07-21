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
	void onPreInit(nc::AppConfiguration &config) override;
	void onInit() override;
	void onFrameStart() override;

#ifdef __ANDROID__
	void onTouchDown(const nc::TouchEvent &event) override;
#endif
	void onKeyReleased(const nc::KeyboardEvent &event) override;
	void onMouseButtonPressed(const nc::MouseEvent &event) override;

  private:
	static const unsigned int NumTextures = 4;
	static const unsigned int NumSprites = 8;

	float angle_;

	bool withViewport_;
	nctl::UniquePtr<nc::Viewport> viewport_;
	nctl::UniquePtr<nc::SceneNode> rootNode_;
	nctl::UniquePtr<nc::Sprite> vpSprite_;
	nctl::UniquePtr<nc::Shader> vpBlurShader_;

	nctl::StaticArray<nctl::UniquePtr<nc::Texture>, NumTextures> textures_;
	nctl::UniquePtr<nc::Font> font_;
	nctl::UniquePtr<nctl::String> debugString_;
	nctl::UniquePtr<nc::TextNode> debugText_;

	nctl::UniquePtr<nc::Shader> spriteShader_;
	nctl::StaticArray<nctl::UniquePtr<nc::Sprite>, NumSprites> sprites_;

	nctl::UniquePtr<nc::Shader> meshShader_;
	nctl::UniquePtr<nc::MeshSprite> meshSprite_;

	void setupViewport();
	void checkClick(float x, float y);
};

#endif
