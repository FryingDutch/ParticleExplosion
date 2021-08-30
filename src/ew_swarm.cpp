#include "EW_Swarm.h"

namespace ParticleExplosion
{
	std::vector<EW_Swarm*> EW_Swarm::swarms;

	EW_Swarm::EW_Swarm(const int _SWARMSIZE, double _xpos, double _ypos) :
		SWARMSIZE(_SWARMSIZE)
	{
		particles = new EW_Particle[SWARMSIZE] ;

		for (int i = 0; i < SWARMSIZE; i++) {
			particles[i].init(_xpos, _ypos);
		}

		swarms.push_back(this);
	}

	EW_Swarm::~EW_Swarm()
	{
		delete[] particles;
	}
}