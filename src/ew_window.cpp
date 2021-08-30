#include "EW_Window.h"
#include "EW_Particle.h"
#include "EW_Swarm.h"
#include <iostream>
#include <cstring>

namespace ParticleExplosion
{
	unsigned int EW_Window::interval = SDL_GetTicks();

	EW_Window::EW_Window()
	{
		this->isRunning = this->init();
	}

	//destroy pointers and close underlying SDL subsystems
	EW_Window::~EW_Window()
	{
		delete[]m_buffer;
		delete[]m_buffer2;

		SDL_DestroyTexture(this->texture);
		SDL_DestroyRenderer(this->renderer);
		SDL_DestroyWindow(this->window);
		SDL_Quit();
	}

	//initialize everything SDL need and create a buffer for all the pixels on the screens
	bool EW_Window::init(const char* _title)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;

		this->window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (this->window == nullptr) return false;

		this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_PRESENTVSYNC);
		if (this->renderer == nullptr) return false;

		this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
		if (this->texture == nullptr) return false;

		m_buffer = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
		memset(this->m_buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

		m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
		memset(this->m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

		return true;
	}

	//bitshift to fill every byte with the correct value of the color and assign it to the correct pixel on the scrren
	void EW_Window::setOnePixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue)
	{
		if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
		Uint32 color = 0;

		color += red;
		color <<= 8;
		color += green;
		color <<= 8;
		color += blue;
		color <<= 8;
		color += 0xFF;

		this->m_buffer[(y * SCREEN_WIDTH) + x] = color;
	}

	//allow the user to set a different speed of transisition in color
	void EW_Window::setPixels(double r_speed, double g_speed, double b_speed)
	{
		static double _r_speed = r_speed / 10000;
		static double _g_speed = g_speed / 10000;
		static double _b_speed = b_speed / 10000;

		static int elapsedTime;
		elapsedTime = SDL_GetTicks();

		//this will transisiton the total value between 0 and 255 and therefore allow a graduate transistion in color
		unsigned char red = (1 + sin(elapsedTime * _r_speed)) * 128;
		unsigned char green = (1 + sin(elapsedTime * _g_speed)) * 128;
		unsigned char blue = (1 + sin(elapsedTime * _b_speed)) * 128;

		for (int j = 0; j < EW_Swarm::swarms.size(); j++)
		{
			EW_Particle* p_particles = EW_Swarm::swarms[j]->getParticles();

			for (unsigned int i = 0; i < EW_Swarm::swarms[j]->SWARMSIZE; i++)
			{
				p_particles[i].update();

				//recast the position in a integer to find its position on screen 
				int x = (p_particles[i].xPos + 1) * SCREEN_WIDTH / 2;
				int y = p_particles[i].yPos * SCREEN_WIDTH / 2 + SCREEN_HEIGHT / 2;

				this->setOnePixel(x, y, red, green, blue);
			}
		}
	}

	//blur the pixels by coloring the surrounding pixels with the current color of the pixel only faded
	void EW_Window::blur()
	{
		Uint32* _tempBuffer = m_buffer;
		m_buffer = m_buffer2;
		m_buffer2 = _tempBuffer;

		for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			for (int x = 0; x < SCREEN_WIDTH; x++)
			{
				int redTotal{ 0 };
				int greenTotal{ 0 };
				int blueTotal{ 0 };

				Uint8 _red{ 0 };
				Uint8 _green{ 0 };
				Uint8 _blue{ 0 };

				//check surrounding pixels
				for (int row = -1; row < 1; row++)
				{
					for (int col = -1; col <= 1; col++)
					{
						int currentX = x + col;
						int currentY = y + row;

						if (currentX >= 0 && currentX < SCREEN_WIDTH && currentY >= 0 && currentY < SCREEN_HEIGHT)
						{
							//extract color information
							Uint32 _color = m_buffer2[(currentY * SCREEN_WIDTH) + currentX];
							_red = _color >> 24;
							_green = _color >> 16;
							_blue = _color >> 8;

							redTotal += _red;
							greenTotal += _green;
							blueTotal += _blue;
						}
					}
				}

				//blur it
				_red = redTotal / 6;
				_green = greenTotal / 6;
				_blue = blueTotal / 6;

				this->setOnePixel(x, y, _red, _green, _blue);
			}
		}
	}

	//update current frame
	void EW_Window::update()
	{
		this->lastTime = this->currentTime;
		this->currentTime = SDL_GetTicks();
		EW_Window::interval = (this->currentTime - this->lastTime) / 10;

		this->blur();
		SDL_UpdateTexture(this->texture, NULL, this->m_buffer, SCREEN_WIDTH * sizeof(Uint32));
		SDL_RenderClear(this->renderer);
		SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
		SDL_RenderPresent(this->renderer);
	}

	//poll events
	void EW_Window::pollEvents()
	{
		static SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) this->isRunning = false;
			if (event.type == SDL_MOUSEMOTION) SDL_GetMouseState(&this->mouseXpos, &this->mouseYpos);
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					this->breath();
				}
			}
		}
	}

	void EW_Window::breath()
	{
		EW_Swarm::swarms[1] = new EW_Swarm{900, (double)0.9f, (double)this->mouseYpos };
	}
}