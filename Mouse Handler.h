#pragma once

#include <utility>
#include <unordered_map>
#include <SDL3/SDL.h>

#include "CONSTANTS.h"
#include "COMMAND.h"

struct Master_Handler;

struct Mouse_Handler {
	
private:

	bool enable_mouse = ENABLE_MOUSE;
	float& x;
	float& y;
	Master_Handler& MasterHandler;

public:

	std::unordered_map<SDL_MouseButtonFlags, COMMAND_ID> mouseBindings{};

	void harvestMouseState();

	Mouse_Handler(std::pair<float, float>& cursor, Master_Handler& varMasH);


	void processCommand(const Command& command);
	
	private:
		void processAppCommand(const Command& command);
			void processMouseCommand(const Command& command);
};