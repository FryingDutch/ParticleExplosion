#include "EW_Particle.h"
#include "EW_Window.h"
#include <math.h>

namespace ParticleExplosion
{
	EW_Particle::EW_Particle(double _xpos, double _ypos)
	{
		this->init(_xpos, _ypos);
	}

	//set random initial values
	void EW_Particle::init(double _xpos = 0, double _ypos = 0)
	{
		this->xPos = _xpos;
		this->yPos = _ypos;

		this->direction = (2 * M_PI * rand()) / RAND_MAX;
		this->speed = (0.05 * rand()) / RAND_MAX;

		this->speed *= this->speed;
	}

	//move and adjust direction 
	void EW_Particle::update()
	{
		this->direction += EW_Window::interval * 0.002;

		double _xspeed = this->speed * cos(this->direction);
		double _yspeed = this->speed * sin(this->direction);

		this->yPos += _yspeed * EW_Window::interval;
		this->xPos += _xspeed * EW_Window::interval;

		if (xPos < -1.1f || xPos > 1.f || yPos < -1.f || yPos > 1.1f) this->init();
	}
}
