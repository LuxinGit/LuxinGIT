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

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
}
void GUI_Handler::initialiseBinding(const Command_Definition& definition) {
    if (!definition.guiBinding)
        return;

    const auto header =
        static_cast<size_t>(definition.guiBinding->header);

    headers[header].push_back(definition.command.ID);

    GUI_METADATA binding = *definition.guiBinding;
    switch (binding.functionType) {
        case GUI_METADATA::FUNCTION_TYPE::SLIDER:
        {
            GUI_METADATA::SLIDER_METADATA& sMD = std::get<GUI_METADATA::SLIDER_METADATA>(binding.metadata);
            sMD.underlying = sMD.getUnderlying(MasterHandler);
            break;
        }
        case GUI_METADATA::FUNCTION_TYPE::COLOUR:
        {
            GUI_METADATA::COLOUR_METADATA& cMD = std::get<GUI_METADATA::COLOUR_METADATA>(binding.metadata);
            cMD.underlying = cMD.getUnderlying(MasterHandler);
            break;
        }
        }
    labelMapping.emplace(definition.command.ID, std::move(binding));
}

void GUI_Handler::beginFrame() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();

    ImGui::NewFrame();
}

void GUI_Handler::menu(const std::string& name, const std::vector<COMMAND_ID>& commands) {
    if (ImGui::BeginMenu(name.c_str())) {
        for (const auto& command : commands) menuItem(command);
        ImGui::EndMenu();
    }
}
    void GUI_Handler::menuItem(const COMMAND_ID& ID) {
        using type = GUI_METADATA::FUNCTION_TYPE;
    
        GUI_METADATA& md = labelMapping.at(ID);

        switch (md.functionType) {
        case type::BINARY:
            binaryMenuItem(ID, md);
            break;
        default:
            nonbinaryMenuItem(ID, md);
            break;
        }
    }
        void GUI_Handler::binaryMenuItem(const COMMAND_ID& ID, GUI_METADATA& md) {
            if (ImGui::MenuItem(labelMapping.at(ID).label.data())) {
                MasterHandler.CommandHandler.constructCommand(ID);
            }
        }
        void GUI_Handler::nonbinaryMenuItem(const COMMAND_ID& ID, GUI_METADATA& md) {
        
            if (ImGui::MenuItem(md.label.data())) {
                if (openPopouts.contains(ID))
                    openPopouts.erase(ID);
                else
                    openPopouts.insert(ID);
            }

        }

void GUI_Handler::checkForPopouts() {
    std::vector<COMMAND_ID> clearlist = {};
    for (const COMMAND_ID& ID : openPopouts) {
        if (!renderPopout(ID)) {
            clearlist.emplace_back(ID);
        }
    }
    for (const COMMAND_ID& cID : clearlist) {
        openPopouts.erase(cID);
    }
}
    bool GUI_Handler::renderPopout(const COMMAND_ID& ID) {
        using type = GUI_METADATA::FUNCTION_TYPE;
        GUI_METADATA& md = labelMapping.at(ID);
        bool open = true;
        ImGui::Begin(md.label.data(), &open);
        
        switch (md.functionType) {
        case type::BINARY:
            return true;
            break;
        case type::SLIDER:
            sliderPopout(ID, md);
            break;
        case type::COLOUR:
            colourPopout(ID, md);
            break;
        default:
            return true;
        }
        ImGui::End();
        return open;
    }
        void GUI_Handler::sliderPopout(const COMMAND_ID& ID, GUI_METADATA& md) {
            GUI_METADATA::SLIDER_METADATA& sMD = std::get<GUI_METADATA::SLIDER_METADATA>(md.metadata);
            int payload = *sMD.underlying;

            if (ImGui::SliderInt(
                md.label.data(),
                &payload,
                sMD.minimum,
                sMD.maximum))
            {
                MasterHandler.CommandHandler.constructCommand(ID, payload);
            }

        }
        void GUI_Handler::colourPopout(const COMMAND_ID& ID, GUI_METADATA& md) {
            GUI_METADATA::COLOUR_METADATA& cMD = std::get<GUI_METADATA::COLOUR_METADATA>(md.metadata);
            std::array<float, 4> displayload = luxel::coloursToFloat(*cMD.underlying);
            if (ImGui::ColorEdit4(md.label.data(), displayload.data()))
                MasterHandler.CommandHandler.constructCommand(ID, luxel::floatsToColour(displayload));
            int rgba[4] = {
                static_cast<int>((*cMD.underlying)[0]),
                static_cast<int>((*cMD.underlying)[1]),
                static_cast<int>((*cMD.underlying)[2]),
                static_cast<int>((*cMD.underlying)[3])
            };
            if (ImGui::InputInt4("RGBA", rgba)) {
                std::array<uint8_t, 4> ret = {};
                for (int i = 0; i < 4; i++)
                    ret[i] = std::clamp(rgba[i], 0, 255);
                MasterHandler.CommandHandler.constructCommand(ID, ret);
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

void GUI_Handler::render() {
    
    renderMMBar();
    checkForPopouts();
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(
        ImGui::GetDrawData(),
        Renderer
    );
}
