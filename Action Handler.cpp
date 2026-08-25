#include "Action Handler.h"
#include "Canvas Handler.h"

void Action_Handler::pixelChange(luxel* p, const std::array<uint8_t, 4>& originalColour) {
        currentAction.addLuxel(p, originalColour);
}
void Action_Handler::checkForActions() {
    if (!currentAction.changeSet.empty()) addNewAction();
}
void Action_Handler::addNewAction() {
	actionQueueIndex++;
	actionQueue.resize(actionQueueIndex);
	actionQueue.emplace_back(std::move(currentAction));
	currentAction = {};
}
void Action_Handler::processAction() {

	for (auto& change : actionQueue[actionQueueIndex].changeSet) {
        std::swap(change.originalColour, change.p->colour);
	}

}
void Action_Handler::undoAction() {
    if (actionQueueIndex < 0) return;
    processAction();
	actionQueueIndex--;
}
void Action_Handler::redoAction() {
    actionQueueIndex++;
    if (actionQueueIndex == actionQueue.size()) {
        actionQueueIndex--;
        return;
    }
	processAction();
}
void Action_Handler::resetActionQueue() {
    actionQueue = {};
}

void Action_Handler::processCommand(const Command& command) {

    using T = Command::TYPE;

    switch (command.type) {
    case T::APP:
        processAppCommand(command);
        break;
    }

}

    void Action_Handler::processMetaCommand(const Command& command) {

        using A = Command::META::ACTION;

        switch (static_cast<A>(command.action)) {
        case (A::RESET):
            processResetCommand(command);
            break;
        }
    }
        void Action_Handler::processResetCommand(const Command& command) {

            using S = Command::META::RESET;

            switch (static_cast<S>(command.setting)) {
            case (S::RESET_ACTION_QUEUE):
                resetActionQueue();
                break;
            }
        }

    void Action_Handler::processAppCommand(const Command& command) {

    using A = Command::APP::ACTION;

    switch (static_cast<A>(command.action)) {
    case (A::UNDO_REDO):
        processActionCommand(command);
        break;
    }
}
        void Action_Handler::processActionCommand(const Command& command) {

            using S = Command::APP::UNDO_REDO;

            switch (static_cast<S>(command.setting)) {
            case (S::UNDO):
                undoAction();
                break;
            case (S::REDO):
                redoAction();
                break;
            }
}