
#include <SDL3/SDL.h>
#include <imgui.h>
#include "Application.h"

/*
TO DO:
    
    ARGUMENTS SHOULD BE MAP YES GOOD YES GOOD 

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

    Application_State AppState;

    LuxinPaint::initialiseApplicationState(AppState);

    while (LuxinPaint::runFrame(AppState));

    LuxinPaint::cleanup(AppState);

    return 0;

}
