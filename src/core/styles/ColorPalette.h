#pragma once

#include <imgui.h>

// Paleta alineada con el tema oscuro del Sistema POS (core/styles/Theme.cpp)
// y con los botones primarios del módulo de Configuración.
namespace installer::styles
{
    // Superficies
    inline const ImVec4 Background = ImVec4(0.11f, 0.12f, 0.14f, 1.0f);
    inline const ImVec4 Surface    = ImVec4(0.15f, 0.16f, 0.19f, 1.0f);
    inline const ImVec4 SurfaceAlt = ImVec4(0.18f, 0.19f, 0.22f, 1.0f);
    inline const ImVec4 Input      = ImVec4(0.13f, 0.14f, 0.16f, 1.0f);
    inline const ImVec4 Card       = ImVec4(0.13f, 0.15f, 0.19f, 1.0f);
    inline const ImVec4 CardAlt    = ImVec4(0.10f, 0.12f, 0.16f, 1.0f);

    // Bordes
    inline const ImVec4 Border     = ImVec4(0.28f, 0.30f, 0.34f, 1.0f);
    inline const ImVec4 Separator  = ImVec4(0.28f, 0.30f, 0.34f, 0.60f);

    // Texto
    inline const ImVec4 Text         = ImVec4(0.90f, 0.91f, 0.93f, 1.0f);
    inline const ImVec4 TextMuted    = ImVec4(0.58f, 0.60f, 0.64f, 1.0f);
    inline const ImVec4 TextDisabled = ImVec4(0.42f, 0.44f, 0.48f, 1.0f);

    // Acento (barra lateral, selección) y primario (botones de acción)
    inline const ImVec4 Accent       = ImVec4(0.25f, 0.47f, 0.72f, 1.0f);
    inline const ImVec4 AccentHover  = ImVec4(0.32f, 0.55f, 0.80f, 1.0f);
    inline const ImVec4 AccentActive = ImVec4(0.20f, 0.40f, 0.62f, 1.0f);

    inline const ImVec4 Primary      = ImVec4(0.10f, 0.52f, 0.96f, 1.0f);
    inline const ImVec4 PrimaryHover = ImVec4(0.16f, 0.60f, 1.00f, 1.0f);
    inline const ImVec4 PrimaryDown  = ImVec4(0.08f, 0.40f, 0.78f, 1.0f);

    // Estados
    inline const ImVec4 Success = ImVec4(0.30f, 0.65f, 0.45f, 1.0f);
    inline const ImVec4 Warning = ImVec4(0.85f, 0.65f, 0.25f, 1.0f);
    inline const ImVec4 Danger  = ImVec4(0.86f, 0.35f, 0.36f, 1.0f);
    inline const ImVec4 Info    = ImVec4(0.35f, 0.55f, 0.75f, 1.0f);

    inline ImVec4 withAlpha(ImVec4 color, float alpha)
    {
        color.w = alpha;
        return color;
    }
}
