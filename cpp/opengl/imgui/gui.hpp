#ifndef IMGUI_DISABLED
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

#ifndef IMGUI_DISABLED
ImGuiIO &initImgui(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return ImGui::GetIO();
}
#endif

#ifndef IMGUI_DISABLED
void renderImgui(ImGuiIO& io, bool *showDemo) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (*showDemo) {
        ImGui::ShowDemoWindow(showDemo);
    }

    ImGui::Begin("Editor");
    // ImGui::Text("Hello from ImGui!");
    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::Checkbox("Show demo", showDemo);      // Edit bools storing our window open/close state
    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
#endif