#pragma once

#include "Command.h"

struct Canvas_Handler;
struct luxel;

struct Action_Handler {

private:

	std::unordered_map<size_t, std::array<uint8_t, 4>> Action = {};
	int actionQueueIndex = 0;
	Canvas_Handler& CanvasHandler;

	std::vector<std::unordered_map<size_t, std::array<uint8_t, 4>>> actionQueue;

	void addNewAction();
	void undoAction();
	void redoAction();
	void processAction();

public: 

	Action_Handler(Canvas_Handler& varcanvH) : CanvasHandler(varcanvH) {}

	void pixelChange(const coordinate& c, const std::array<uint8_t, 4>& originalColour); // Draw_Handler's method for adding to action
	void checkForActions();// Command_Handler's method for adding a new action.

	void processCommand(const Command& command);
private:
		void processAppCommand(const Command& command);
			void processActionCommand(const Command& command);


};