#pragma once

#include "Command.h"
#include "Application/STRUCTS.h"
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
	using Action = std::variant<Change_Set, Command::Cmmd>;	
	
	Change_Set currentAction;	
	std::vector<Action> actionQueue;
	int actionQueueIndex = 0;
};

namespace Action {
	void markChangedPixel(Action_State&, luxel*, const colour& originalColour);
	void commitCurrentAction(Action_State&);

	void processHistory(Application_State& s, Command::Cmmd& command);
	void processClearActionQueue(Application_State& mh, Command::Cmmd&); 
}
