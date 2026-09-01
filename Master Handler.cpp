#include "Master Handler.h"
#include "COMMAND REPO.h"

Master_Handler::Master_Handler(int varwidth, int varheight) :
    width(varwidth), height(varheight),
    CanvasHandler(width, height, *this),
    CommandHandler(*this),
    KeyboardHandler(*this),
    MouseHandler(CanvasHandler.CursorHandler.deltaCursor, *this),
    SDLHandler(*this),
    CLIHandler(*this),
    GUIHandler(*this)
{
    initialiseBindings();
    SDLHandler.setReferences(GUIHandler, &MouseHandler.enableMouse);
    if (ENABLE_CLI) CLIHandler.beginCLILoop();
};


void Master_Handler::renderFrame() {
    KeyboardHandler.harvestKeyboardState();
    MouseHandler.harvestMouseState();
    CommandHandler.processCommands();
    SDLHandler.renderFrame();
}

void Master_Handler::cleanup() const {
    SDLHandler.cleanup();
}

void Master_Handler::initialiseBindings() {
    for (const auto& def : COMMAND_REPO) {

        const COMMAND_ID ID = def.command.ID;
        COMMAND_ID_DEF_MAP.emplace(ID, &def);

        if (def.keyBinding)
            KeyboardHandler.keyBindings.emplace(*def.keyBinding, ID);

        if (def.mouseBinding)
            MouseHandler.mouseBindings.emplace(*def.mouseBinding, ID);

        if (def.cliBinding) {
            CLIHandler.stringMapping.emplace(*def.cliBinding, ID);
        }

        GUIHandler.initialiseBinding(def);
    }
}