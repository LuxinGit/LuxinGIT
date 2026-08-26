#include "GUI Handler.h"

GUI_Handler::GUI_Handler(Master_Handler& varMH) :
    MasterHandler(varMH)
{
    initialise();
}

void GUI_Handler::initialise()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(Window, Renderer);
    ImGui_ImplSDLRenderer3_Init(Renderer);
}

void GUI_Handler::beginFrame() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();

    ImGui::NewFrame();
}

void GUI_Handler::render() {
    ImGui::Render();

    ImGui_ImplSDLRenderer3_RenderDrawData(
        ImGui::GetDrawData(),
        Renderer
    );
}
