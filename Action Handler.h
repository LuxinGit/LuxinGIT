#pragma once

#include "Command.h"
#include <unordered_set>
struct Canvas_Handler;

struct Change_Set {
	struct Change {
		luxel* l;
		std::array<uint8_t, 4> originalColour;
	};

	std::vector<Change> changeSet;
	std::unordered_set<luxel*> changedLuxels;

	void addLuxel(luxel* l, const std::array<uint8_t, 4>& oC) {
		if (changedLuxels.insert(l).second)
			changeSet.emplace_back(l, oC);
	}
};

struct Action_Handler {

private:

	Change_Set currentAction;
	std::vector<Change_Set> actionQueue;
	int actionQueueIndex = 0;
	Canvas_Handler& CanvasHandler;

	void addNewAction();
	void undoAction();
	void redoAction();
	void processAction();



public: 

	Action_Handler(Canvas_Handler& varcanvH) : CanvasHandler(varcanvH) {}

	void pixelChange(luxel* , const std::array<uint8_t, 4>& originalColour); // Draw_Handler's method for adding to action
	void checkForActions();// Command_Handler's method for adding a new action.
	void resetActionQueue();

	void processCommand(const Command& command);
private:

	void processMetaCommand(const Command& command);
		void processResetCommand(const Command& command);
	void processAppCommand(const Command& command);
		void processActionCommand(const Command& command);
				


};