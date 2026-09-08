#include "Object.h"
#include "Application/Application.h"




namespace Object {
	
	namespace {
	
	}

	void object::renderObject(const SDL_State& s)
	{
		texture = SDL::createTexture(s, topLeft.x - bottomRight.x, topLeft.x - bottomRight.y);
	}

	object createObjectFromEdit(Canvas_State& cS, Object_Edit& oE) 
	{

		int objW = oE.max.x - oE.min.x + 1;
		int objH = oE.max.y - oE.min.y + 1;

		for (int y = 0; y < objH; ++y) {
			for (int x = 0; x < objW; ++x) {
				coordinate canvasCoord{ oE.min.x + x, oE.min.y + y };
				oE.activeObject->pixels[y * objW + x] = *Canvas::getLuxelFromCoord(cS, canvasCoord);
			}
		}

	}
}