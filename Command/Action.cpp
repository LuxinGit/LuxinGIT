#include "Action.h"
#include "Application/Application.h"

void Change_Set::addLuxel(luxel* l, const std::array<uint8_t, 4>& oC) {
    if (changedLuxels.insert(l).second)
        changeSet.push_back({l, oC});
}

namespace Action {

    namespace {
        void addNewAction(Action_State& s) {
            s.actionQueueIndex++;
            s.actionQueue.resize(s.actionQueueIndex);
            s.actionQueue.emplace_back(std::move(s.currentAction));
            s.currentAction = {};
        }
        void processAction(Application_State& mh, Action_State& s) {

            if (auto* cmd = std::get_if<Command::Cmd>(&s.actionQueue[s.actionQueueIndex])) 
            {
                cmd->processor(mh, *cmd);
            }
            else for (auto& change : std::get<Change_Set>(s.actionQueue[s.actionQueueIndex]).changeSet) {
                std::swap(change.originalColour, change.l->colour);
            }

        }
        void undoAction(Application_State& mh, Action_State& s) {
            if (s.actionQueueIndex < 1) return;
            processAction(mh, s);
            s.actionQueueIndex--;
        }
        void redoAction(Application_State& mh, Action_State& s) {
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
    void processHistory(Application_State& s, Command::Cmd& command) {
        if (std::get<int>(command.args[0]) == 0)
            undoAction(s, s.ActionState);
        else
            redoAction(s, s.ActionState);
    }
    void processClearActionQueue(Application_State& mh, Command::Cmd&)
    {
        mh.ActionState.actionQueue = {}; 
    }
}