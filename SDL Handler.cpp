#include "SDL Handler.h"
#include "Master Handler.h"
#include "imgui.h"

SDL_Handler::SDL_Handler(Master_Handler& varMH) :
	canvas			(varMH.CanvasState.canvas),
	canvasHeight	(varMH.CanvasState.height),
	canvasWidth		(varMH.CanvasState.width),
	MasterHandler	(varMH)

{
	Window		= SDL_CreateWindow(DEFAULT_APPLICATION_NAME, canvasWidth, canvasHeight, 0);
	Renderer	= SDL_CreateRenderer(Window, nullptr);
	initialiseTexture();
}
void SDL_Handler::setReferences(GUI_Handler& varGH, bool* varenableMouse) {
	GUIHandler = &varGH;
	enableMouse = varenableMouse;
	SDL_SetWindowRelativeMouseMode(Window, false);
}
void SDL_Handler::cleanup() const {
	if (Texture) SDL_DestroyTexture(Texture);
	if (Renderer) SDL_DestroyRenderer(Renderer);
	if (Window) SDL_DestroyWindow(Window);
	SDL_Quit();
}
SDL_Handler::~SDL_Handler() {
	cleanup();
}

void SDL_Handler::initialiseTexture() {
	Texture = SDL_CreateTexture(
		Renderer, 
		SDL_PIXELFORMAT_RGBA32, 
		SDL_TEXTUREACCESS_STREAMING, 
		DEFAULT_CANVAS_WIDTH_MAX, 
		DEFAULT_CANVAS_HEIGHT_MAX
	);
	updateTexture();
}
void SDL_Handler::updateTexture() {
	SDL_Rect rect{ 0, 0, canvasWidth, canvasHeight };
	SDL_UpdateTexture(Texture, &rect, canvas.data(), DEFAULT_CANVAS_WIDTH_MAX * sizeof(luxel));
}
void SDL_Handler::renderTexture() const {
	SDL_FRect rect{ 0, 0, static_cast<float>(canvasWidth), static_cast<float>(canvasHeight) };
	SDL_RenderTexture(Renderer, Texture, &rect, nullptr);
}

void SDL_Handler::registerCanvasSizeChange() const {
	SDL_SetWindowSize(Window, canvasWidth, canvasHeight);
}
void SDL_Handler::checkShowMouse() const {
	if (!*enableMouse or ImGui::GetIO().WantCaptureMouse) SDL_ShowCursor();
	else SDL_HideCursor();
}

void SDL_Handler::renderPresent() const {
	SDL_RenderPresent(Renderer);
}
void SDL_Handler::renderCrosshair() const {
	if (MasterHandler.CursorState.enableCrosshair)
	{	
		auto [x, y] = MasterHandler.CursorState.cursor;
		int r = MasterHandler.DrawState.pen;
		SDL_RenderLine(Renderer, x - r, y, x + r, y);
		SDL_RenderLine(Renderer, x, y - r, x, y + r);
	}
};
void SDL_Handler::renderFrame() {
	GUIHandler->beginFrame();
	Cursor::checkCursorData(MasterHandler);
	updateTexture();
	renderTexture();
	renderCrosshair(); // todo  fix the fact that we be rendering the crosshair regardless of whether or not we be using the gui
	GUIHandler->render();
	checkShowMouse();
	renderPresent();
}
