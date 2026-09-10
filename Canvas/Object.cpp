#include "Object.h"
#include "Application/Application.h"




namespace Object {

	//// Helper Functions

	namespace {

		object* retrieveObjectFromIdentifier(Object_State& oS, const Command::argument& a)
		{
			if (const int* id = std::get_if<int>(&a)) {
				for (const auto& object : oS.objects)
					if (object->objectID == *id) return object.get();
			}

			if (const std::string* name = std::get_if<std::string>(&a)) {
				for (const auto& object : oS.objects)
					if (object->name == *name) return object.get();
			}

			return nullptr;
		};

		bool identifyCollision(const Object::object* o, const coordinate& c)
		{
			if (c >= o->topLeft && c < coordinate{ o->topLeft.x + o->width, o->topLeft.y + o->height })
				return true;
			else
				return false;
		}

		object* retrieveObjectFromUnderneathCursor(Object_State& oS, const Cursor_State& cS)
		{
			coordinate c = cS.cursor;

			for (const auto& object : oS.objects)
			{
				if (identifyCollision(object.get(), c))
					return object.get();
			}

			return nullptr;
		}

	}
	
	void selectObject(Application_State& s, Command::Cmd& cmd)
	{
		// Behaviour - Selects an object.
		// if supplied a UiD uses that, otherwise uses the object underneath the cursor.
		// selectedObject always deselected - we use cmd to figure out if we want to set it to something new or not.

		//0:INT selectionMode { 0 / DEFAULT = CURSOR, 1 UiD, 2 Deselect }
		//1:UNFIXED_TYPE<optional> UniqueIdentifier;

		int* selectionMode = std::get_if<int>(&cmd.args[0]);
		object* o = nullptr;

		if (!selectionMode)
		{
			cmd.ID = COMMAND_ID::INVALID;
			cmd.args = { "selectObject", "No selectionMode supplied? " };
			return;
		}

		switch (*selectionMode)
		{
		case 0:
			o = retrieveObjectFromUnderneathCursor(s.ObjectState, s.CursorState);
			break;
		case 1:
			o = retrieveObjectFromIdentifier(s.ObjectState, cmd.args[1]);
			break;
		case 2:
			o = nullptr;
			break;
		default:
			cmd.ID = COMMAND_ID::INVALID;
			cmd.args = { "selectObject", "selectionMode out of range? " };
			return;
		}

		std::swap(s.ObjectState.selectedObject, o);

		if (o)
			cmd.args = { 1, o->objectID };
		else
			cmd.args = { 2 };

	}


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

		void createObjectFromEdit(Canvas_State& cS, Object_Edit& oE) 
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

		object* createNewObject(Object_State& oS, std::string name = "")
		{

			object* o = nullptr;
			oS.objects.push_back(std::make_unique<object>());
			o = oS.objects.back().get();

			o->objectID = ++oS.lastObjectID;

			if(name.empty())
				name = "Unnamed Object " + std::to_string(o->objectID);
			o->name = name;

			return o;

		}

		void initaliseObjectEditMode(Application_State& s, Command::Cmd& cmd)
		{

			//0:UNFIXED_TYPE<optional> UiD

			Object_State& oS = s.ObjectState;
			object* o = nullptr;
			std::string name = "";

			oS.objectEditBuffer = Object_Edit();
			oS.activeObjectEdit = &oS.objectEditBuffer;

			if (!std::holds_alternative<std::monostate>(cmd.args[0]))
			{
				o = retrieveObjectFromIdentifier(oS, cmd.args[0]);

				if (!o)
					if (std::string* n = std::get_if<std::string>(&cmd.args[0]))
						name = *n;
			}

			if (!o)			
				o = createNewObject(oS, name);
				
			oS.objectEditBuffer.activeObject = o;

			emplaceObjectUntoCanvas(s.CanvasState, *o);			

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

	
	void moveObject(Application_State& s, Command::Cmd& cmd)
	{
		//0:
		object* o = s.ObjectState.selectedObject;
		if (!o) o = retrieveObjectFromIdentifier(s.ObjectState, cmd.args[2]);
		if (!o) 
		{
			cmd.ID = COMMAND_ID::INVALID;
			cmd.args = { "moveObject", "No selected object, and no alternative identifier provided" };
			return;
		}

		int setting = std::get<int>(cmd.args[0]);
		coordinate dest = { -1, -1 };
		switch (setting)
		{
		case 0:
			dest = std::get<coordinate>(cmd.args[1]);
			break;
		case 1:
			dest = s.CursorState.cursor;
			break;
		case 2:
			s.CursorState.carryingObject = !s.CursorState.carryingObject;
			if (s.CursorState.carryingObject)
				s.ObjectState.selectedObject = o;
			else
				s.ObjectState.selectedObject = nullptr;
			return;
		default:
			cmd.ID = COMMAND_ID::INVALID;
			cmd.args = { "moveObject", "Setting out of range (max expected == 2)" };
			return;
		}

		o->topLeft = dest;

	}


	object::~object()
	{
		if (texture) SDL_DestroyTexture(texture);
	}

}