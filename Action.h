#pragma once

#include "Command.h"
#include <unordered_set>

struct Change {
	luxel* l;
	std::array<uint8_t, 4> originalColour;
};

struct Change_Set {

	std::vector<Change> changeSet;
	std::unordered_set<luxel*> changedLuxels;

	void addLuxel(luxel* l, const std::array<uint8_t, 4>& oC);
};

struct Action_State {
	Change_Set currentAction;
	//using Action = std::variant<Change_Set, Command::Cmd>;
	std::vector<Change_Set> actionQueue;
	int actionQueueIndex = 0;
};

namespace Action {
	void markChangedPixel(Action_State&, luxel*, const colour& originalColour);
	void commitCurrentAction(Action_State&);
	void processUndo(Master_Handler&, Command::Cmd&);
	void processRedo(Master_Handler&, Command::Cmd&);
	void processClearActionQueue(Master_Handler& mh, Command::Cmd&); 
}
