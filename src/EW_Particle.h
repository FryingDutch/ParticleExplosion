#pragma once
#include <stdlib.h>

namespace ParticleExplosion
{
	struct EW_Particle
	{
	public:
		double xPos{ 0 };
		double yPos{ 0 };

		double direction{ 0 };
		double speed{ 0 };

	public:
		EW_Particle(double _xpos = 0, double _ypos = 0);
		void init(double _xpos, double _ypos);
		void update();
	};
}