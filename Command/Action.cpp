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
        void processAction(Application_State& aS) {

            Action_State& actS = aS.ActionState;

            if (auto* cmd = std::get_if<Command::Cmmd>(&actS.actionQueue[actS.actionQueueIndex])) 
            {
                cmd->processor(aS, *cmd);
            }
            else for (auto& change : std::get<Change_Set>(actS.actionQueue[actS.actionQueueIndex]).changeSet) {
                std::swap(change.originalColour, change.l->colour);
            }

        }
        void undoAction(Application_State& mh, Action_State& s) {
            if (s.actionQueueIndex < 1) return;
            processAction(mh);
            s.actionQueueIndex--;
        }
        void redoAction(Application_State& mh, Action_State& s) {
            if (s.actionQueueIndex + 1 >= static_cast<int>(s.actionQueue.size())) return;
            s.actionQueueIndex++;
            processAction(mh);
        }
    }


    void markChangedPixel(Action_State& s, luxel* l, const colour& originalColour) {
        s.currentAction.addLuxel(l, originalColour);
    }
    void commitCurrentAction(Action_State& s) {
        if (!s.currentAction.changeSet.empty()) addNewAction(s);
    }
    void processHistory(Application_State& s, Command::Cmmd& command) {
        if (std::get<int>(command.args[0]) == 0)
            undoAction(s, s.ActionState);
        else
            redoAction(s, s.ActionState);
    }
    void processClearActionQueue(Application_State& mh, Command::Cmmd&)
    {
        mh.ActionState.actionQueue = {}; 
    }
}

namespace Action::Queue
{

    namespace Processor
    {
        static void processClearActionQueue(Application_State& s, Command::cmd& c)
        {
            s.ActionState.actionQueue = {};
        }
    }

    const Command::dfn ACTION_QUEUE_RESETQUEUE =
    {
        .name = "Reset Action Queue",

        .prcssr = &Processor::processClearActionQueue

    };
}

namespace Action::Undo
{
    
    namespace Processor
    {
        static void processUndo(Application_State& s, Command::cmd&) 
        {
            if (s.ActionState.actionQueueIndex < 1) return;
            processAction(s);
            s.ActionState.actionQueueIndex--;
        }
    }

    const Command::dfn ACTION_UNDO =
    {
        .prcssr = &Processor::processUndo
    };

}

namespace Action::Redo
{
    namespace Processor
    {
        static void processRedo(Application_State& s, Command::cmd&) 
        {
            if (s.ActionState.actionQueueIndex + 1 >= static_cast<int>(s.ActionState.actionQueue.size())) return;
            s.ActionState.actionQueueIndex++;
            processAction(s);
        }
    }

    const Command::dfn ACTION_REDO =
    {
        .prcssr = &Processor::processRedo
    };
}