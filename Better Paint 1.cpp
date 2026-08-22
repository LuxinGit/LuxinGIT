// Better Paint 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <vector>
#include <array>
#include <cmath>
#include <cstdlib>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <variant>

#include "Command.h"
#include "Master Handler.h"

/*
TO DO:

1. Brush modes


*/

int main()
{

    // DEFINES
    int width = 800, height = 600;
    bool running = true;

    Master_Handler MasterHandler(width, height);

    while (running) {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;  
        }

        MasterHandler.processCommands();

    }

    // Clean up
    MasterHandler.cleanup();
    return 0;
}
