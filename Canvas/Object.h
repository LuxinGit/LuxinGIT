#pragma once
#include "Application/STRUCTS.h"
#include <vector>
#include "SDL3/SDL.h"

struct SDL_State;
struct Canvas_State;

namespace Object {
	
	struct object {

		coordinate topLeft = { 0, 0 };
		coordinate bottomRight = { 0, 0 };
		std::vector<luxel> pixels = {};
		
		SDL_Texture* texture = nullptr;
		SDL_FRect area;

		/* IMPLEMENTATION PLAN :

		1.5 set up SDL_Texture to both render textures within obejct state and choose to display them if we hvae an activeTexture or something
		3. move object from texture / whatever -> buffer (for x = 0, x < width, x++) something like that we did it before
		4. then when done, trim until we can fully describe object within rect and convert into texture again.

		*/

		void renderObject(const SDL_State&);

	};

	struct Object_Edit {
		coordinate min;
		coordinate max;
		object* activeObject = nullptr;
	};


	object createObjectFromEdit(Canvas_State& s, Object_Edit& e);

}

struct Object_State {

	std::vector<Object::object> objects;
	Object::Object_Edit* activeObjectEdit = nullptr;

};