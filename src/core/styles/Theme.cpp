#include "Theme.h"
#include "ColorPalette.h"

#include <imgui.h>

namespace installer::styles
{
    void Theme::Apply()
    {
        ImGuiStyle &s = ImGui::GetStyle();

        s.WindowPadding = ImVec2(0.0f, 0.0f);
        s.FramePadding = ImVec2(12.0f, 9.0f);
        s.ItemSpacing = ImVec2(10.0f, 10.0f);
        s.ItemInnerSpacing = ImVec2(8.0f, 6.0f);
        s.IndentSpacing = 18.0f;
        s.ScrollbarSize = 10.0f;
        s.GrabMinSize = 10.0f;

        s.WindowRounding = 0.0f;
        s.ChildRounding = 12.0f;
        s.FrameRounding = 8.0f;
        s.PopupRounding = 8.0f;
        s.ScrollbarRounding = 6.0f;
        s.GrabRounding = 6.0f;
        s.TabRounding = 6.0f;

        s.WindowBorderSize = 0.0f;
        s.ChildBorderSize = 1.0f;
        s.FrameBorderSize = 1.0f;
        s.PopupBorderSize = 1.0f;

        s.ButtonTextAlign = ImVec2(0.5f, 0.5f);

        ImVec4 *c = s.Colors;

        c[ImGuiCol_Text] = Text;
        c[ImGuiCol_TextDisabled] = TextDisabled;
        c[ImGuiCol_WindowBg] = Background;
        c[ImGuiCol_ChildBg] = ImVec4(0, 0, 0, 0);
        c[ImGuiCol_PopupBg] = SurfaceAlt;
        c[ImGuiCol_Border] = Border;
        c[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

        c[ImGuiCol_FrameBg] = Input;
        c[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.16f, 0.19f, 1.0f);
        c[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.16f, 0.19f, 1.0f);

        c[ImGuiCol_TitleBg] = Surface;
        c[ImGuiCol_TitleBgActive] = Surface;
        c[ImGuiCol_TitleBgCollapsed] = Surface;
        c[ImGuiCol_MenuBarBg] = Surface;

        c[ImGuiCol_ScrollbarBg] = ImVec4(0, 0, 0, 0);
        c[ImGuiCol_ScrollbarGrab] = ImVec4(1, 1, 1, 0.16f);
        c[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1, 1, 1, 0.28f);
        c[ImGuiCol_ScrollbarGrabActive] = ImVec4(1, 1, 1, 0.38f);

        c[ImGuiCol_CheckMark] = Primary;
        c[ImGuiCol_SliderGrab] = Primary;
        c[ImGuiCol_SliderGrabActive] = PrimaryHover;

        c[ImGuiCol_Button] = Primary;
        c[ImGuiCol_ButtonHovered] = PrimaryHover;
        c[ImGuiCol_ButtonActive] = PrimaryDown;

        c[ImGuiCol_Header] = SurfaceAlt;
        c[ImGuiCol_HeaderHovered] = AccentHover;
        c[ImGuiCol_HeaderActive] = Accent;

        c[ImGuiCol_Separator] = Separator;
        c[ImGuiCol_SeparatorHovered] = AccentHover;
        c[ImGuiCol_SeparatorActive] = Accent;

        c[ImGuiCol_TextSelectedBg] = withAlpha(Primary, 0.35f);
        c[ImGuiCol_NavHighlight] = Primary;
        c[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.55f);
    }
}
