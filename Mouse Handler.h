#pragma once

#include <utility>
#include "CONSTANTS.h"

struct Command_Handler;

struct Mouse_Handler {
	
private:

	bool enable_mouse = ENABLE_MOUSE;
	float& x;
	float& y;
	Command_Handler& CommandHandler;

public:

	void harvestMouseState();
	void processMouseAppCommand();

	Mouse_Handler(std::pair<float, float>& cursor, Command_Handler& varCommH);
	
};