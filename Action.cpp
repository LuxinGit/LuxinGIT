#include "Action.h"
#include "Canvas Handler.h"
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
        void processAction(Action_State& s) {

            for (auto& change : s.actionQueue[s.actionQueueIndex].changeSet) {
                std::swap(change.originalColour, change.l->colour);
            }

        }
        void undoAction(Action_State& s) {
            if (s.actionQueueIndex < 1) return;
            processAction(s);
            s.actionQueueIndex--;
        }
        void redoAction(Action_State& s) {
            if (s.actionQueueIndex + 1 >= static_cast<int>(s.actionQueue.size())) return;
            s.actionQueueIndex++;
            processAction(s);
        }
    }


    void markChangedPixel(Action_State& s, luxel* l, const std::array<uint8_t, 4>& originalColour) {
        s.currentAction.addLuxel(l, originalColour);
    }
    void commitCurrentAction(Action_State& s) {
        if (!s.currentAction.changeSet.empty()) addNewAction(s);
    }
    void processUndo(Master_Handler& mh, Command&) {
        undoAction(mh.ActionState);
    }
    void processRedo(Master_Handler& mh, Command&) {
        redoAction(mh.ActionState);
    }
    void processClearActionQueue(Master_Handler& mh, Command&)
    {
        mh.ActionState.actionQueue = {}; 
    }
}