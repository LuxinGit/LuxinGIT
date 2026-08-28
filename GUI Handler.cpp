#include "GUI Handler.h"
#include "Master Handler.h"

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
void GUI_Handler::initialiseBinding(Command_Definition definition) {
    if (!definition.guiBinding)
        return;

    const auto header =
        static_cast<size_t>(definition.guiBinding->header);

    headers[header].push_back(definition.command.ID);
    labelMapping.emplace(definition.command.ID, &*definition.guiBinding);
}

void GUI_Handler::beginFrame() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();

    ImGui::NewFrame();
}

void GUI_Handler::menuItem(COMMAND_ID ID) {
    using type = GUI_METADATA::FUNCTION_TYPE;
    
    const GUI_METADATA* md = labelMapping.at(ID);

    switch (md->functionType) {
    case type::BINARY:
        binaryMenuItem(ID, md);
        break;
    case type::SLIDER:
        sliderMenuItem(ID, md);
        break;
    }
}
    void GUI_Handler::binaryMenuItem(COMMAND_ID ID, const GUI_METADATA* md) {
        if (ImGui::MenuItem(labelMapping.at(ID)->label.data())) {
            MasterHandler.CommandHandler.constructCommand(ID);
        }
    }
    void GUI_Handler::sliderMenuItem(COMMAND_ID ID, const GUI_METADATA* md) {
        int min = 1, max = 100; // SHOULDNT BE DEFINED HERE, NEEDS TO BE ADDED TO GUI_METADATA
        ImGui::Begin(labelMapping.at(ID)->label.data());
        int payload;

        if (ImGui::SliderInt(labelMapping.at(ID)->label.data(), &payload, min, max))
            MasterHandler.CommandHandler.constructCommand(ID, payload);
        ImGui::End();
    }

void GUI_Handler::menu(const std::string& name, const std::vector<COMMAND_ID>& commands) {
    if (ImGui::BeginMenu(name.c_str())) {
        for (const auto& command : commands) menuItem(command);
        ImGui::EndMenu();
    }
}




void GUI_Handler::renderMMBar() {
    if (ImGui::BeginMainMenuBar()) {
        
        menu("File",    headers[static_cast<int>(GUI_METADATA::HEADER::FILE)]);
        menu("Edit",    headers[static_cast<int>(GUI_METADATA::HEADER::EDIT)]);
        menu("Tools",   headers[static_cast<int>(GUI_METADATA::HEADER::TOOLS)]);
        ImGui::EndMainMenuBar();
    }

}

bool GUI_Handler::wantsMouse() {
    return ImGui::GetIO().WantCaptureMouse;
}
void GUI_Handler::render() {
    
    renderMMBar();
    
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(
        ImGui::GetDrawData(),
        Renderer
    );
}
