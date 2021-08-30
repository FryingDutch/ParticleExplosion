#pragma once
#include <SDL.h>
#include <vector>
#include "EW_Swarm.h"

namespace ParticleExplosion
{
	class EW_Window
	{
	public:
		const static unsigned int SCREEN_WIDTH{ 1200 };
		const static unsigned int SCREEN_HEIGHT{ 700 };

		int mouseXpos{ 0 };
		int mouseYpos{ 0 };

		bool isRunning{ false };
		static unsigned int interval;

	private:	
		unsigned int lastTime;
		unsigned int currentTime;

		Uint32* m_buffer;
		Uint32* m_buffer2;

	public:
		SDL_Window* window{ nullptr };
		SDL_Renderer* renderer{ nullptr };
		SDL_Texture* texture{ nullptr };

		EW_Swarm mainSwarm{1600};
		EW_Swarm varSwarm1{0};

	private:
		void setOnePixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);

	public:
		EW_Window();
		~EW_Window();

		void setPixels(double r_speed = 3, double g_speed = 4, double b_speed = 5);
		bool init(const char* _title = "Particle Explosion");
		void blur();
		void update();
		void breath();
		void pollEvents();
	};
}