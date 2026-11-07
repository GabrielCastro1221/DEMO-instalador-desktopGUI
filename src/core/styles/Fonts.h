#pragma once

#include <imgui.h>

namespace installer::styles
{
    // Familia tipográfica del instalador (Inter). Si no se encuentran los archivos
    // se usan fuentes del sistema (Ubuntu, Noto Sans, Liberation, DejaVu) y, como
    // último recurso, la fuente interna de Dear ImGui.
    struct FontSet
    {
        ImFont *regular = nullptr;
        ImFont *medium = nullptr;
        ImFont *semibold = nullptr;
    };

    constexpr float kBaseFontSize = 16.0f;

    FontSet &Fonts();

    // Debe llamarse después de ImGui::CreateContext() y antes del primer NewFrame().
    bool LoadFonts(ImGuiIO &io);
}
