#include "Object.h"
#include "Platform/SDLWrapper.h"


void Object::object::renderObject(SDL_State& s)
{
	texture = SDL::createTexture(s, topLeft.x - bottomRight.x, topLeft.x - bottomRight.y);
	
}
