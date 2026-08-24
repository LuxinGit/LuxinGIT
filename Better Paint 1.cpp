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

1. Fill has out of boundary issue.
2. Probably just rework coords entirely.
3. Undo / redo functionality.
4. Some better way to select colours.
5. drawBaseball();
6. fix mouse cursor click with pendown not drawing point 
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
