#pragma once
#include "EW_Particle.h"
#include <vector>

namespace ParticleExplosion
{
	class EW_Swarm
	{
	public:
		EW_Particle* particles;

	public:
		const int SWARMSIZE;
		static std::vector<EW_Swarm*> swarms;

	public:
		EW_Swarm(const int _SWARMSIZE = 20000, double _xpos = 0, double _ypos = 0);
		~EW_Swarm();

		EW_Particle* getParticles() { return particles; }
	};
}