#pragma once

#include <utility>
#include <unordered_map>
#include <SDL3/SDL.h>

#include "CONSTANTS.h"
#include "COMMAND.h"

struct Master_Handler;

struct Mouse_Handler {
	
private:

	float& x;
	float& y;
	Master_Handler& MasterHandler;

public:

	bool enableMouse = ENABLE_MOUSE;

	std::unordered_map<SDL_MouseButtonFlags, COMMAND_ID> mouseBindings{};

	void harvestMouseState();

	Mouse_Handler(std::pair<float, float>& cursor, Master_Handler& varMasH);

	void processCommand(const Command_OLD& command);
	
	private:
		void processAppCommand(const Command_OLD& command);
			void processMouseCommand(const Command_OLD& command);
};