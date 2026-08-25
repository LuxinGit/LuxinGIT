#include "Action Handler.h"
#include "Canvas Handler.h"

void Action_Handler::pixelChange(const coordinate& c, const std::array<uint8_t, 4>& originalColour) {
    Action.try_emplace(
        CanvasHandler.indexFromCoord(c),
        originalColour
    );
}
void Action_Handler::checkForActions() {
    if (!Action.empty()) addNewAction();
}
void Action_Handler::addNewAction() {
	actionQueueIndex++;
	actionQueue.resize(actionQueueIndex);
	actionQueue.emplace_back(std::move(Action));
	Action = {};
}
void Action_Handler::processAction() {
	std::unordered_map<size_t, std::array<uint8_t, 4>> tAction = actionQueue[actionQueueIndex];
	for (auto& [index, originalColour] : actionQueue[actionQueueIndex]) {
		std::array<uint8_t, 4>& newColour = CanvasHandler.getLuxelFromIndex(index)->colour;
        tAction[index] = newColour;
        newColour = originalColour;
	}
	actionQueue[actionQueueIndex] = std::move(tAction);
}
void Action_Handler::undoAction() {
    if (actionQueueIndex < 0) return;
    processAction();
	actionQueueIndex--;
}
void Action_Handler::redoAction() {
    if (actionQueueIndex++ == actionQueue.size()) return;
	processAction();
}

void Action_Handler::processCommand(const Command& command) {

    using T = Command::TYPE;

    switch (command.type) {
    case T::APP:
        processAppCommand(command);
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