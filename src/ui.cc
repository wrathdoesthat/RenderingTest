#include "ui.hh"

bool set_pos{false};
TextEditor editor;

ImFont* font1{nullptr};
ImFont* font2{nullptr};
ImFont* font3{nullptr};
ImFont* font4{nullptr};

void prepare_imgui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    editor.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());

    font1 = io.Fonts->AddFontFromFileTTF("fonts/consola.ttf", 12);
    font2 = io.Fonts->AddFontFromFileTTF("fonts/consolab.ttf", 12);
    font3 = io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 14);
    font4 = io.Fonts->AddFontFromFileTTF("fonts/RobotoMono-Medium.ttf", 16);
    io.Fonts->Build();
}

void render_ui(GLFWwindow* window, UIData& ui_data) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowSize(ImVec2(0, 0));

    ImGui::PushFont(font3);

    if (ui_data.show_ui) {
        if (!set_pos) {
            ImVec2 window_center = ImGui::GetMainViewport()->GetCenter();
            ImVec2 window_size = ImGui::GetWindowSize();
            ImVec2 actual_center =
                ImVec2(window_center.x - (window_size.x / 2), window_center.y);

            ImGui::SetNextWindowPos(actual_center);
            set_pos = true;
        }

        ImGuiWindowFlags flags{0};
        flags |= ImGuiWindowFlags_NoTitleBar;
        flags |= ImGuiWindowFlags_NoResize;

        ImGui::Begin("Wind1", &ui_data.show_ui, flags);
        ImGui::Checkbox("Postprocess", &ui_data.do_postprocess);
        ImGui::End();
    }

    ImGui::SetWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
    ImGui::PopFont();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void setup_ui_style() {
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 0;
    style.FrameRounding = 0;
    style.ScrollbarRounding = 0;
    style.ScrollbarSize = 3;
    style.WindowBorderSize = 0;

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.071, 0.071, 0.071, 1.0);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.141, 0.141, 0.141, 1.0);

    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.071, 0.071, 0.071, 1.0);

    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2, 0.2, 0.2, 1.0);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.31, 0.482, 0.51, 1.0);

    style.Colors[ImGuiCol_Button] = ImVec4(0.141, 0.141, 0.141, 1.0);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2, 0.2, 0.2, 1.0);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.31, 0.482, 0.51, 1.0);

    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.31, 0.482, 0.51, 1.0);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31, 0.482, 0.51, 1.0);
};