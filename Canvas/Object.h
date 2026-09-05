#pragma once
#include "Application/STRUCTS.h"
#include <vector>
#include "SDL3/SDL.h"

struct SDL_State;

namespace Object {
	
	struct object {

		coordinate topLeft = { 0, 0 };
		coordinate bottomRight = { 0, 0 };
		std::vector<luxel> pixels = {};
		
		SDL_Texture* texture = nullptr;
		SDL_Rect area;

		void renderObject(SDL_State&);

		/* IMPLEMENTATION PLAN :

		1. set up Canvas_State to have 2 canvas, with *activeCanvas (similar to activeColour)
		1.5 set up SDL_Texture to both render textures within obejct state and choose to display them if we hvae an activeTexture or something
		2. :) fun refactor the yes for just making sure anything that usees canvas now dereferences it instead.
		3. move object from texture / whatever -> buffer (for x = 0, x < width, x++) something like that we did it before
		4. then when done, trim until we can fully describe object within rect and convert into texture again.

		*/
	};

	

}

struct Objects_State {
	std::vector<Object::object> objects;
};