#pragma once

#include <imgui.h>

#include <cstddef>

// Biblioteca de componentes visuales del instalador. Reproduce el lenguaje del
// Sistema POS (tarjetas, campos con título superior, botones primarios/secundarios,
// avisos con barra lateral) usando una sola fuente tipográfica.
namespace installer::ui
{
    enum class Kind
    {
        Info,
        Success,
        Warning,
        Danger
    };

    enum class Icon
    {
        Building,
        User,
        Database,
        Install,
        Check,
        Shield
    };

    enum class StepState
    {
        Pending,
        Active,
        Done
    };

    // ---- Utilidades ------------------------------------------------------------
    ImU32 u32(const ImVec4 &color);
    ImVec2 measure(ImFont *font, float size, const char *text, float wrapWidth = 0.0f);

    // ---- Texto -----------------------------------------------------------------
    // Escriben en la posición actual del cursor y avanzan como cualquier widget.
    void label(ImFont *font, float size, const ImVec4 &color, const char *text);
    void paragraph(ImFont *font, float size, const ImVec4 &color, const char *text, float wrapWidth = 0.0f);
    void sectionLabel(const char *text);

    // ---- Superficies -----------------------------------------------------------
    void cardBegin(const char *id, ImVec2 size = ImVec2(0.0f, 0.0f), float padX = 24.0f, float padY = 22.0f);
    void cardEnd();
    void callout(Kind kind, const char *title, const char *body);

    // ---- Formularios -----------------------------------------------------------
    // width <= 0 usa todo el ancho disponible; con un valor positivo permite
    // colocar dos campos en la misma fila.
    void field(const char *caption, const char *id, char *buffer, std::size_t size, const char *hint = "",
               bool required = false, const char *error = nullptr, const char *helper = nullptr, float width = 0.0f);
    void passwordField(const char *caption, const char *id, char *buffer, std::size_t size, bool &reveal,
                       bool required = false, const char *error = nullptr, const char *helper = nullptr, float width = 0.0f);
    bool checkbox(const char *id, const char *labelText, bool *value);

    // ---- Botones y estados -----------------------------------------------------
    bool primaryButton(const char *text, ImVec2 size, bool enabled = true);
    bool secondaryButton(const char *text, ImVec2 size, bool enabled = true);
    void badge(const char *text, const ImVec4 &color);
    void progressBar(float fraction, float height = 10.0f);

    // ---- Dibujo ----------------------------------------------------------------
    void drawLogoMark(ImDrawList *dl, ImVec2 center, float radius, ImU32 accent, ImU32 foreground);
    void drawIcon(ImDrawList *dl, Icon icon, ImVec2 center, float size, ImU32 color, float thickness = 1.8f);
    void drawCheck(ImDrawList *dl, ImVec2 center, float size, ImU32 color, float thickness = 2.0f);
    void drawSpinner(ImDrawList *dl, ImVec2 center, float radius, ImU32 color, float thickness = 2.4f);
    void drawStepMarker(ImDrawList *dl, ImVec2 center, float radius, StepState state, int number);

    // Fuerza de contraseña: 0 (vacía) a 4.
    int passwordStrength(const char *password);
}
