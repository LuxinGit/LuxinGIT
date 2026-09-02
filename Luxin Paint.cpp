// Better Paint 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SDL3/SDL.h>
#include <imgui.h>
#include "COMMAND.h"
#include "Master Handler.h"

/*
TO DO:
    
    cursor needs some better way to show on dark backgrounds.
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

    Master_Handler MasterHandler;

    while (running) {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if(ENABLE_GUI) ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT)
                running = false;  
        }

        MasterHandler.renderFrame();

    }

    MasterHandler.cleanup();
    return 0;
}
