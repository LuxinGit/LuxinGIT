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

#include "COMMAND.h"
#include "Master Handler.h"

/*
TO DO:

1. clearCanvas needs to be moved within /undo functionality
2. Some better way to select colours. //include eyedropper ?
3. drawBaseball();
*/

int main()
{
    bool running = true;

    Master_Handler MasterHandler(DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT);

    while (running) {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;  
        }

        MasterHandler.processCommands();

    }

    MasterHandler.cleanup();
    return 0;
}
