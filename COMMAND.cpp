#include "COMMAND.h"
#include "Master Handler.h"

#include <variant>
#include <unordered_map>
#include <array>

Command_OLD::Command_OLD(COMMAND_ID varID, Command_OLD::TYPE vartype, int varaction, int varsetting, bool varRepeatable, Command_OLD::Payload_OLD varPayload) :
    type(vartype), action(varaction), setting(varsetting), repeatable(varRepeatable), ID(varID), payload(varPayload) {} //cpp
    Command_OLD::Command_OLD(COMMAND_ID varID, MOVE cmd, bool repeatable, Payload_OLD payload)
        : Command_OLD(
            varID,
            TYPE::MOVE,
            static_cast<int>(cmd.action),
            static_cast<int>(cmd.setting),
            repeatable,
            payload
        )
    {}
    Command_OLD::Command_OLD(COMMAND_ID varID, DRAW cmd, bool repeatable, Payload_OLD payload)
        : Command_OLD(
            varID,
            TYPE::DRAW,
            static_cast<int>(cmd.action),
            static_cast<int>(cmd.setting),
            repeatable,
            payload
        )
    {}
    Command_OLD::Command_OLD(COMMAND_ID varID, META cmd, bool repeatable, Payload_OLD payload)
        : Command_OLD(
            varID,
            TYPE::META,
            static_cast<int>(cmd.action),
            static_cast<int>(cmd.setting),
            repeatable,
            payload
        )
    {}
    Command_OLD::Command_OLD(COMMAND_ID varID, APP cmd, bool repeatable, Payload_OLD payload)
        : Command_OLD(
            varID,
            TYPE::APP,
            static_cast<int>(cmd.action),
            static_cast<int>(cmd.setting),
            repeatable,
            payload
        )
    {}


Command_OLD::MOVE::MOVE(ACTION varAction, int varSetting) : action(varAction), setting(varSetting) {}
    Command_OLD::MOVE::MOVE(DIRECTION varSetting)
        : MOVE(ACTION::DIRECTION, static_cast<int>(varSetting)) {}
    Command_OLD::MOVE::MOVE(SET varSetting)
        : MOVE(ACTION::SET, static_cast<int>(varSetting)) {}


Command_OLD::DRAW::DRAW(ACTION varAction, int varSetting)
    : action(varAction), setting(varSetting) {}
    Command_OLD::DRAW::DRAW(LINE varSetting)
        : DRAW(ACTION::LINE, static_cast<int>(varSetting)) {}
    Command_OLD::DRAW::DRAW(CIRCLE varSetting)
        : DRAW(ACTION::CIRCLE, static_cast<int>(varSetting)) {}
    Command_OLD::DRAW::DRAW(POINT varSetting)
        : DRAW(ACTION::POINT, static_cast<int>(varSetting)) {}
    Command_OLD::DRAW::DRAW(FILL varSetting)
        : DRAW(ACTION::FILL, static_cast<int>(varSetting)) {}


Command_OLD::META::META(ACTION varAction, int varSetting)
    : action(varAction), setting(varSetting) {}
    Command_OLD::META::META(RESET varSetting)
        : META(ACTION::RESET, static_cast<int>(varSetting)) {}
    Command_OLD::META::META(CHANGE_COLOUR varSetting)
        : META(ACTION::CHANGE_COLOUR, static_cast<int>(varSetting)) {}
    Command_OLD::META::META(CHANGE_DRAWSTEP varSetting)
        : META(ACTION::CHANGE_DRAWSTEP, static_cast<int>(varSetting)) {}
    Command_OLD::META::META(CHANGE_PENMODE varSetting)
        : META(ACTION::CHANGE_PENMODE, static_cast<int>(varSetting)) {}
    Command_OLD::META::META(CHANGE_PEN_WIDTH varSetting)
        : META(ACTION::CHANGE_PEN_WIDTH, static_cast<int>(varSetting)) {}
    Command_OLD::META::META(SAVE_ORIGIN varSetting)
        : META(ACTION::SAVE_ORIGIN, static_cast<int>(varSetting)) {}
    Command_OLD::META::META(CANVAS_CHANGE_SIZE varSetting)
        : META(ACTION::CANVAS_CHANGE_SIZE, static_cast<int>(varSetting)) {}


Command_OLD::APP::APP(ACTION varAction, int varSetting) : action(varAction), setting(varSetting) {}
    Command_OLD::APP::APP(INPUT_MODE varSetting) : APP(ACTION::INPUT_MODE, static_cast<int>(varSetting)) {}
    Command_OLD::APP::APP(UNDO_REDO varSetting) : APP(ACTION::UNDO_REDO, static_cast<int>(varSetting)) {}

int* GUI_METADATA::SLIDER_METADATA::resolvePenWidth(Master_Handler& varMH) {
    return &varMH.DrawState.pen;
}
int* GUI_METADATA::SLIDER_METADATA::resolveDrawstep(Master_Handler& varMH) {
    return &varMH.CursorState.drawStep;
}
int* GUI_METADATA::SLIDER_METADATA::resolveCanvasWidth(Master_Handler& varMH)
{
    return &varMH.CanvasState.width;
}
int* GUI_METADATA::SLIDER_METADATA::resolveCanvasHeight(Master_Handler& varMH)
{
    return &varMH.CanvasState.height;
}

colour* GUI_METADATA::COLOUR_METADATA::resolveDrawColourChange(Master_Handler& varMH) {
    return &varMH.DrawState.drawColour;
}
colour* GUI_METADATA::COLOUR_METADATA::resolveBackgroundColourChange(Master_Handler& varMH) {
    return &varMH.DrawState.backgroundColour;
}

namespace Command {
    int& resolvePenWidth(Master_Handler& varMH) {
        return varMH.DrawState.pen;
    }
    int& resolveDrawstep(Master_Handler& varMH) {
        return varMH.CursorState.drawStep;
    }
    int& resolveCanvasWidth(Master_Handler& varMH)
    {
        return varMH.CanvasState.width;
    }
    int& resolveCanvasHeight(Master_Handler& varMH)
    {
        return varMH.CanvasState.height;
    }
    colour& resolveDrawColourChange(Master_Handler& varMH) 
    {
        return varMH.DrawState.drawColour;
    }
    colour& resolveBackgroundColourChange(Master_Handler& varMH) 
    {
        return varMH.DrawState.backgroundColour;
    }
}