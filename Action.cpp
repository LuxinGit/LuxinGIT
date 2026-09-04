#include "Action.h"
#include "Canvas.h"
#include "Master Handler.h"

void Change_Set::addLuxel(luxel* l, const std::array<uint8_t, 4>& oC) {
    if (changedLuxels.insert(l).second)
        changeSet.emplace_back(l, oC);
}

namespace Action {

    namespace {
        void addNewAction(Action_State& s) {
            s.actionQueueIndex++;
            s.actionQueue.resize(s.actionQueueIndex);
            s.actionQueue.emplace_back(std::move(s.currentAction));
            s.currentAction = {};
        }
        void processAction(Master_Handler& mh, Action_State& s) {

            if (auto* cmd = std::get_if<Command::Cmd>(&s.actionQueue[s.actionQueueIndex])) 
            {
                cmd->processor(mh, *cmd);
            }
            else for (auto& change : std::get<Change_Set>(s.actionQueue[s.actionQueueIndex]).changeSet) {
                std::swap(change.originalColour, change.l->colour);
            }

        }
        void undoAction(Master_Handler& mh, Action_State& s) {
            if (s.actionQueueIndex < 1) return;
            processAction(mh, s);
            s.actionQueueIndex--;
        }
        void redoAction(Master_Handler& mh, Action_State& s) {
            if (s.actionQueueIndex + 1 >= static_cast<int>(s.actionQueue.size())) return;
            s.actionQueueIndex++;
            processAction(mh, s);
        }
    }


    void markChangedPixel(Action_State& s, luxel* l, const colour& originalColour) {
        s.currentAction.addLuxel(l, originalColour);
    }
    void commitCurrentAction(Action_State& s) {
        if (!s.currentAction.changeSet.empty()) addNewAction(s);
    }
    void processUndo(Master_Handler& mh, Command::Cmd&) {
        undoAction(mh, mh.ActionState);
    }
    void processRedo(Master_Handler& mh, Command::Cmd&) {
        redoAction(mh, mh.ActionState);
    }
    void processClearActionQueue(Master_Handler& mh, Command::Cmd&)
    {
        mh.ActionState.actionQueue = {}; 
    }
}