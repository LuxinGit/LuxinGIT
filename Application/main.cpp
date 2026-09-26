
#include <SDL3/SDL.h>
#include <imgui.h>
#include "Application.h"

/*
TO DO:
    
    Continue Command_Repo refactor.
    Take #defines out from CONSTANTS.h and work out better way to do it.
    Use smart pointers for Action (Action.h) such that no horrific vector memory realloc every time large actions added.
    Rewrite CLI construction code.    

*/

int main()
{

    Application_State AppState;

    LuxinPaint::initialiseApplicationState(AppState);

    while (LuxinPaint::runFrame(AppState));

    LuxinPaint::cleanup(AppState);

    return 0;

}
