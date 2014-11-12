#ifndef CLASS_NCINE_PARTICLESYSTEM
#define CLASS_NCINE_PARTICLESYSTEM

#include <ctime>
#include <cstdlib>

#include "Array.h"
#include "DrawableNode.h"
#include "ParticleAffectors.h"

namespace ncine {

class Texture;
class Vector2f;
class Particle;
class Rect;

/// The class representing a particle system
class ParticleSystem : public DrawableNode
{
  public:
	// Constructs a particle system made of the specified maximum amount of particles
	ParticleSystem(SceneNode* parent, unsigned int count, Texture *texture, Rect texRect);
	~ParticleSystem();

	/// Adds a particle affector
	void addAffector(ParticleAffector* affector) { affectors_.insertBack(affector); }
	/// Emits an amount of particles with a specified initial life and velocity
	void emitParticles(unsigned int amount, float life, const Vector2f &vel);

	/// Gets the local space flag of the system
	inline bool inLocalSpace(void) const { return inLocalSpace_; }
	/// Sets the local space flag of the system
	inline void setInLocalSpace(bool inLocalSpace) { inLocalSpace_ = inLocalSpace; }

	virtual void update(float interval);

	inline static ObjectType sType() { return PARTICLESYSTEM_TYPE; }

  private:
	/// The particle pool size
	unsigned int poolSize_;
	/// The index of the next free particle in the pool
	unsigned int poolTop_;
	/// The pool containing available particles (only dead ones)
	Particle **particlePool_;
	/// The array containing every particle (dead or alive)
	Particle **particleList_;

	/// The array of particle affectors
	Array<ParticleAffector *> affectors_;

	/// Should the system simulate in local space?
	bool inLocalSpace_;

	virtual void updateRenderCommand() { }
};

/// Returns a random float between x0 and x1
inline float randBetween(float x0, float x1) { return x0 + (x1 - x0) * (rand() / float(RAND_MAX)); }

}

#endif
