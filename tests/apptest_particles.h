#ifndef CLASS_MYEVENTHANDLER
#define CLASS_MYEVENTHANDLER

#include <ncine/IAppEventHandler.h>
#include <ncine/IInputEventHandler.h>
#include <ncine/Vector2.h>
#include <ncine/TimeStamp.h>

namespace ncine {

class AppConfiguration;
class Texture;
class ParticleSystem;

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
	void onTouchMove(const nc::TouchEvent &event) override;
	void onAcceleration(const nc::AccelerometerEvent &event) override;
#endif
	void onKeyReleased(const nc::KeyboardEvent &event) override;
	void onMouseButtonPressed(const nc::MouseEvent &event) override;
	void onMouseMoved(const nc::MouseState &state) override;

	void onJoyMappedAxisMoved(const nc::JoyMappedAxisEvent &event) override;
	void onJoyMappedButtonReleased(const nc::JoyMappedButtonEvent &event) override;
	void onJoyDisconnected(const nc::JoyConnectionEvent &event) override;

  private:
	static const unsigned int NumParticles = 50;

	bool pause_;
	nctl::UniquePtr<nc::Texture> texture_;
	nctl::UniquePtr<nc::ParticleSystem> particleSystem_;
	nc::TimeStamp lastEmissionTime_;
	nc::Vector2f emitVector_;
	nc::Vector2f prevEmitVector_;

	nc::Vector2f joyVectorLeft_;
	nc::Vector2f joyVectorRight_;
};

#endif
