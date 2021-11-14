
namespace ImGui {

    void init_style()
    {
        ImGui::GetStyle().ChildRounding = 10.0f;
        //ImGui::GetStyle().ChildBorderSize = 3.0f;

        ImVec4* colors = ImGui::GetStyle().Colors;
        ImGui::GetStyle().FrameRounding = 4.0f;
        ImGui::GetStyle().WindowPadding.x = 10.0f;
        colors[ImGuiCol_WindowBg] = ImVec4(0.10, 0.61, 0.75, 1.0f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.0, 0.0, 0.0, 1.0f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.0, 0.0, 0.40f, 1.0f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.0, 0.0, 0.40f, 0.3f);
        colors[ImGuiCol_Button] = ImVec4(0.0, 0.0, 0.40f, 0.35f);

        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0, 0.2, 0.75f, 0.6);

        colors[ImGuiCol_PopupBg] = ImVec4(0.45f, 0.45f, 0.08f, 1.0f);
        //colors[ImGuiCol_Button] = ImVec4(0.45f, 0.45f, 0.08f, 1.0f);

    }
}
