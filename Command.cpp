#include "COMMAND.h"

#include <variant>
#include <unordered_map>
#include <array>

Command::Command(COMMAND_ID varID, Command::TYPE vartype, int varaction, int varsetting, bool varRepeatable, Command::Payload varPayload) :
    type(vartype), action(varaction), setting(varsetting), repeatable(varRepeatable), ID(varID), payload(varPayload) {} //cpp
    Command::Command(COMMAND_ID varID, MOVE cmd, bool repeatable, Payload payload)
        : Command(
            varID,
            TYPE::MOVE,
            static_cast<int>(cmd.action),
            static_cast<int>(cmd.setting),
            repeatable,
            payload
        )
    {}
    Command::Command(COMMAND_ID varID, DRAW cmd, bool repeatable, Payload payload)
        : Command(
            varID,
            TYPE::DRAW,
            static_cast<int>(cmd.action),
            static_cast<int>(cmd.setting),
            repeatable,
            payload
        )
    {}
    Command::Command(COMMAND_ID varID, META cmd, bool repeatable, Payload payload)
        : Command(
            varID,
            TYPE::META,
            static_cast<int>(cmd.action),
            static_cast<int>(cmd.setting),
            repeatable,
            payload
        )
    {}
    Command::Command(COMMAND_ID varID, APP cmd, bool repeatable, Payload payload)
        : Command(
            varID,
            TYPE::APP,
            static_cast<int>(cmd.action),
            static_cast<int>(cmd.setting),
            repeatable,
            payload
        )
    {}


Command::MOVE::MOVE(ACTION varAction, int varSetting) : action(varAction), setting(varSetting) {}
    Command::MOVE::MOVE(DIRECTION varSetting)
        : MOVE(ACTION::DIRECTION, static_cast<int>(varSetting)) {}
    Command::MOVE::MOVE(SET varSetting)
        : MOVE(ACTION::SET, static_cast<int>(varSetting)) {}


Command::DRAW::DRAW(ACTION varAction, int varSetting)
    : action(varAction), setting(varSetting) {}
    Command::DRAW::DRAW(LINE varSetting)
        : DRAW(ACTION::LINE, static_cast<int>(varSetting)) {}
    Command::DRAW::DRAW(CIRCLE varSetting)
        : DRAW(ACTION::CIRCLE, static_cast<int>(varSetting)) {}
    Command::DRAW::DRAW(POINT varSetting)
        : DRAW(ACTION::POINT, static_cast<int>(varSetting)) {}
    Command::DRAW::DRAW(FILL varSetting)
        : DRAW(ACTION::FILL, static_cast<int>(varSetting)) {}


Command::META::META(ACTION varAction, int varSetting)
    : action(varAction), setting(varSetting) {}
    Command::META::META(RESET varSetting)
        : META(ACTION::RESET, static_cast<int>(varSetting)) {}
    Command::META::META(CHANGE_COLOUR varSetting)
        : META(ACTION::CHANGE_COLOUR, static_cast<int>(varSetting)) {}
    Command::META::META(CHANGE_DRAWSTEP varSetting)
        : META(ACTION::CHANGE_DRAWSTEP, static_cast<int>(varSetting)) {}
    Command::META::META(CHANGE_PENMODE varSetting)
        : META(ACTION::CHANGE_PENMODE, static_cast<int>(varSetting)) {}
    Command::META::META(CHANGE_PEN_WIDTH varSetting)
        : META(ACTION::CHANGE_PEN_WIDTH, static_cast<int>(varSetting)) {}
    Command::META::META(SAVE_ORIGIN varSetting)
        : META(ACTION::SAVE_ORIGIN, static_cast<int>(varSetting)) {}


Command::APP::APP(ACTION varAction, int varSetting) : action(varAction), setting(varSetting) {}
    Command::APP::APP(INPUT_MODE varSetting) : APP(ACTION::INPUT_MODE, static_cast<int>(varSetting)) {}
    Command::APP::APP(UNDO_REDO varSetting) : APP(ACTION::UNDO_REDO, static_cast<int>(varSetting)) {}