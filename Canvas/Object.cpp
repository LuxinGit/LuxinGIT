#include "Object.h"
#include "Application/Application.h"




namespace Object {
	
	namespace {
		void createObjectTexture(SDL_State& s, Object::object& o)
		{
			if (o.texture) SDL_DestroyTexture(o.texture);
			o.texture = SDL::createTexture(s, o.width, o.height);
			SDL_UpdateTexture(o.texture, nullptr, o.pixels.data(), o.width * sizeof(luxel));			
		}
	

		void emplaceObjectUntoCanvas(Canvas_State& cS, object& o)
		{
			if (!Canvas::newCoordCheck(coordinate{ cS.width, cS.height }, o.topLeft))
				o.topLeft = coordinate{ 0, 0 };

			for (int y = 0; y < o.height; y++) {
				for (int x = 0; x < o.width; x++) {

					luxel* target = Canvas::getDisplayedLuxelFromActiveCanvas(cS, coordinate{ o.topLeft.x + x, o.topLeft.y + y });
					if (!target) continue;

					target->colour = o.pixels[y * o.width + x].colour;
				}
			}
		}

		void  createObjectFromEdit(Canvas_State& cS, Object_Edit& oE) 
		{
			object& o = *oE.activeObject;
			o.width = oE.max.x - oE.min.x + 1;
			o.height = oE.max.y - oE.min.y + 1;			
			
			o.pixels.resize(o.width * o.height);

			for (int y = 0; y < o.height; ++y) {
				for (int x = 0; x < o.width; ++x) {
					coordinate canvasCoord{ oE.min.x + x, oE.min.y + y };
					oE.activeObject->pixels[y * o.width + x] = *Canvas::getDisplayedLuxelFromActiveCanvas(cS, canvasCoord);
				}
			}

			oE.activeObject->topLeft = coordinate{ oE.min };
		
		}

		void initaliseObjectEditMode(Application_State& s, Command::Cmd& cmd)
		{
			//0:UNFIXED_TYPE ObjectIndex

			Object_State& oS = s.ObjectState;
			if (oS.activeObjectEdit) return;

			int* objID = std::get_if<int>(&cmd.args[0]);
			std::string* objname = std::get_if<std::string>(&cmd.args[0]);
			// objID can not be 0.
			if (objname) {
				int& id = oS.nameMap[*objname];

				if (id)
					objID = &id;
			}

			oS.objectEditBuffer = Object_Edit();
			oS.activeObjectEdit = &oS.objectEditBuffer;

			if (objID)
			{
				oS.objectEditBuffer.activeObject = &oS.objects[*objID];
			}
			else 
			{
				int newID = oS.lastObjectID++;
				object& o = oS.objects[newID];
				o.objectID = newID;

				if (objname)
					o.name = *objname;
				else
					o.name += " " + std::to_string(newID);
				oS.objectEditBuffer.activeObject = &o;

				oS.nameMap[o.name] = newID;

			}

			emplaceObjectUntoCanvas(s.CanvasState, *oS.objectEditBuffer.activeObject);

		}

		void destroyObjectEditMode(Application_State& s) 
			// in the future we could pass args here, if calling from gui or whatever.
			// but for now no rush
		{
			Object_State& oS = s.ObjectState;

			createObjectFromEdit(s.CanvasState, *oS.activeObjectEdit);
			createObjectTexture(s.SDLState, *oS.activeObjectEdit->activeObject);
			std::fill(
				s.CanvasState.bufferCanvas.begin(),
				s.CanvasState.bufferCanvas.end(),
				luxel{ colour{ 0, 0, 0, 0 } }
			);
			oS.activeObjectEdit = nullptr;

		}

	}

	void toggleObjectEditMode(Application_State& s, Command::Cmd& cmd)
	{
		if(s.ObjectState.activeObjectEdit)
		{ 
			destroyObjectEditMode(s);
			Canvas::swapActiveCanvas(s.CanvasState);
		}
		else
		{
			Canvas::swapActiveCanvas(s.CanvasState);
			initaliseObjectEditMode(s, cmd);
		}
	}


}