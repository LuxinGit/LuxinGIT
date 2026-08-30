// Better Paint 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SDL3/SDL.h>
#include <imgui.h>
#include "COMMAND.h"
#include "Master Handler.h"

/*
TO DO:

1. Save commands from command queue for later use
2. Some better way to select colours. //include eyedropper ?
3. drawBaseball(); drawCock();
4. accept combo commands on keybindings.
5. canvas resizing <-
6. 3d

*/

int main()
{
    bool running = true;

    Master_Handler MasterHandler(DEFAULT_CANVAS_WIDTH_CUR, DEFAULT_CANVAS_HEIGHT_CUR);

    while (running) {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT)
                running = false;  
        }

        MasterHandler.renderFrame();

    }

    MasterHandler.cleanup();
    return 0;
}
