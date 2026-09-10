#pragma once
#include "Application/STRUCTS.h"
#include "Command/Command.h"
#include <vector>
#include <unordered_map>
#include "SDL3/SDL.h"
#include <memory>

struct SDL_State;
struct Canvas_State;

namespace Object {
	
	struct object {

		// object metadata

		int objectID = 0;
		std::string name = "Unnamed Object";

		// data
		coordinate topLeft = { 0, 0 };
		int width = 0;
		int height = 0;
		std::vector<luxel> pixels = {};
		
		SDL_Texture* texture = nullptr;

		/* IMPLEMENTATION PLAN :

		1.5 set up SDL_Texture to both render textures within obejct state and choose to display them if we hvae an activeTexture or something
		3. move object from texture / whatever -> buffer (for x = 0, x < width, x++) something like that we did it before
		4. then when done, trim until we can fully describe object within rect and convert into texture again.

		*/

		~object();

	};

	struct Object_Edit 
	{
		coordinate min = coordinate{ DEFAULT_CANVAS_WIDTH_MAX, DEFAULT_CANVAS_HEIGHT_MAX };
		coordinate max = coordinate{ -1 , -1 };
		object* activeObject = nullptr;
	};

	void toggleObjectEditMode(Application_State& s, Command::Cmd& cmd);

	void moveObject(Application_State& s, Command::Cmd& cmd);

}

struct Object_State {


	std::vector<std::unique_ptr<Object::object>> objects;

	int lastObjectID = 0;

	Object::Object_Edit  objectEditBuffer;
	Object::Object_Edit* activeObjectEdit = nullptr;

	Object::object*		 selectedObject = nullptr;

};