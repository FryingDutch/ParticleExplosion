#include <iostream>
#include <math.h>
#include "EW_Window.h"

using namespace ParticleExplosion;

int main(int argc, char* argv[])
{
    srand(time(NULL));

    EW_Window window;

    while (window.isRunning)
    {
        window.pollEvents();
        window.update();
        window.setPixels(3,2,1);
    }
    return 0;
}
