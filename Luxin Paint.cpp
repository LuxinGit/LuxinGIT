// Better Paint 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SDL3/SDL.h>
#include <imgui.h>
#include "COMMAND.h"
#include "Master Handler.h"

/*
TO DO:
    
    keybind reduce / increase brightness
    keybind remappings
    Save commands from command queue for later use
    drawBaseball(); drawCock();
    accept combo commands on keybindings.
    3d

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
