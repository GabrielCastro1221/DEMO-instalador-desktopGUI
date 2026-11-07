#include "UiKit.h"

#include "core/styles/ColorPalette.h"
#include "core/styles/Fonts.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace installer::ui
{
    namespace pal = installer::styles;

    namespace
    {
        constexpr float kPi = 3.14159265358979f;

        ImFont *fontRegular() { return pal::Fonts().regular; }
        ImFont *fontMedium() { return pal::Fonts().medium; }
        ImFont *fontSemibold() { return pal::Fonts().semibold; }

        struct KindColors
        {
            ImVec4 accent;
            ImVec4 fill;
            ImVec4 border;
        };

        KindColors colorsFor(Kind kind)
        {
            ImVec4 base = pal::Info;
            switch (kind)
            {
            case Kind::Success:
                base = pal::Success;
                break;
            case Kind::Warning:
                base = pal::Warning;
                break;
            case Kind::Danger:
                base = pal::Danger;
                break;
            case Kind::Info:
                base = pal::Primary;
                break;
            }
            return {base, pal::withAlpha(base, 0.10f), pal::withAlpha(base, 0.35f)};
        }

        // Título del campo sobre el control. Devuelve el ancho disponible.
        float drawCaption(const char *caption, bool required, const char *trailing, bool *trailingClicked, float explicitWidth)
        {
            ImDrawList *dl = ImGui::GetWindowDrawList();
            const ImVec2 origin = ImGui::GetCursorScreenPos();
            const float width = explicitWidth > 0.0f ? explicitWidth : ImGui::GetContentRegionAvail().x;
            const float size = 13.0f;
            const ImVec2 captionSize = measure(fontMedium(), size, caption);

            dl->AddText(fontMedium(), size, origin, u32(pal::TextMuted), caption);
            if (required)
                dl->AddText(fontMedium(), size, ImVec2(origin.x + captionSize.x + 3.0f, origin.y), u32(pal::Danger), "*");

            if (trailing && trailingClicked)
            {
                const ImVec2 trailingSize = measure(fontMedium(), size, trailing);
                const ImVec2 linkPos(origin.x + width - trailingSize.x, origin.y);
                ImGui::SetCursorScreenPos(ImVec2(linkPos.x - 4.0f, origin.y - 3.0f));
                ImGui::PushID(trailing);
                *trailingClicked = ImGui::InvisibleButton("##trailing", ImVec2(trailingSize.x + 8.0f, size + 8.0f));
                const bool hovered = ImGui::IsItemHovered();
                ImGui::PopID();
                if (hovered)
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                dl->AddText(fontMedium(), size, linkPos, u32(hovered ? pal::PrimaryHover : pal::Primary), trailing);
            }

            ImGui::SetCursorScreenPos(origin);
            ImGui::Dummy(ImVec2(width, size + 6.0f));
            return width;
        }

        void drawFieldOutline(bool active, bool hasError)
        {
            ImDrawList *dl = ImGui::GetWindowDrawList();
            if (hasError)
                dl->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), u32(pal::Danger), 8.0f, 0, 1.5f);
            else if (active)
                dl->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), u32(pal::Primary), 8.0f, 0, 1.5f);
        }

        void drawFieldNote(const char *error, const char *helper, float width)
        {
            if (error && error[0])
            {
                ImGui::Dummy(ImVec2(0.0f, 6.0f));
                paragraph(fontRegular(), 12.5f, pal::Danger, error, width);
            }
            else if (helper && helper[0])
            {
                ImGui::Dummy(ImVec2(0.0f, 6.0f));
                paragraph(fontRegular(), 12.5f, pal::TextMuted, helper, width);
            }
        }
    }

    // ---------------------------------------------------------------------------
    ImU32 u32(const ImVec4 &color)
    {
        return ImGui::ColorConvertFloat4ToU32(color);
    }

    ImVec2 measure(ImFont *font, float size, const char *text, float wrapWidth)
    {
        return font->CalcTextSizeA(size, FLT_MAX, wrapWidth, text);
    }

    void label(ImFont *font, float size, const ImVec4 &color, const char *text)
    {
        const ImVec2 origin = ImGui::GetCursorScreenPos();
        const ImVec2 extent = measure(font, size, text);
        ImGui::GetWindowDrawList()->AddText(font, size, origin, u32(color), text);
        ImGui::Dummy(ImVec2(extent.x, std::max(extent.y, size)));
    }

    void paragraph(ImFont *font, float size, const ImVec4 &color, const char *text, float wrapWidth)
    {
        const float width = wrapWidth > 0.0f ? wrapWidth : ImGui::GetContentRegionAvail().x;
        const ImVec2 origin = ImGui::GetCursorScreenPos();
        const ImVec2 extent = measure(font, size, text, width);
        ImGui::GetWindowDrawList()->AddText(font, size, origin, u32(color), text, nullptr, width);
        ImGui::Dummy(ImVec2(width, extent.y));
    }

    void sectionLabel(const char *text)
    {
        label(fontSemibold(), 12.0f, pal::Primary, text);
    }

    // ---- Superficies ---------------------------------------------------------
    void cardBegin(const char *id, ImVec2 size, float padX, float padY)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 12.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(padX, padY));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 8.0f));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, pal::Card);
        ImGui::PushStyleColor(ImGuiCol_Border, pal::Border);

        ImGuiChildFlags flags = ImGuiChildFlags_Borders;
        if (size.y <= 0.0f)
            flags |= ImGuiChildFlags_AutoResizeY;

        ImGui::BeginChild(id, size, flags, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    }

    void cardEnd()
    {
        ImGui::EndChild();
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(4);
    }

    void callout(Kind kind, const char *title, const char *body)
    {
        const KindColors colors = colorsFor(kind);
        ImDrawList *dl = ImGui::GetWindowDrawList();

        const float width = ImGui::GetContentRegionAvail().x;
        const float padX = 18.0f;
        const float padY = 14.0f;
        const float titleSize = 14.0f;
        const float bodySize = 13.5f;
        const float wrap = width - padX * 2.0f - 6.0f;

        const ImVec2 bodyExtent = measure(fontRegular(), bodySize, body, wrap);
        const float height = padY * 2.0f + titleSize + 5.0f + bodyExtent.y;

        const ImVec2 p0 = ImGui::GetCursorScreenPos();
        const ImVec2 p1(p0.x + width, p0.y + height);

        dl->AddRectFilled(p0, p1, u32(colors.fill), 10.0f);
        dl->AddRect(p0, p1, u32(colors.border), 10.0f, 0, 1.0f);
        dl->AddRectFilled(ImVec2(p0.x, p0.y + 10.0f), ImVec2(p0.x + 3.0f, p1.y - 10.0f), u32(colors.accent), 2.0f);

        dl->AddText(fontSemibold(), titleSize, ImVec2(p0.x + padX + 2.0f, p0.y + padY), u32(colors.accent), title);
        dl->AddText(fontRegular(), bodySize, ImVec2(p0.x + padX + 2.0f, p0.y + padY + titleSize + 5.0f), u32(pal::Text), body, nullptr, wrap);

        ImGui::Dummy(ImVec2(width, height));
    }

    // ---- Formularios ---------------------------------------------------------
    void field(const char *caption, const char *id, char *buffer, std::size_t size, const char *hint,
               bool required, const char *error, const char *helper, float width)
    {
        const bool hasError = error && error[0];

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 0.0f));
        const float fieldWidth = drawCaption(caption, required, nullptr, nullptr, width);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(14.0f, 11.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, hasError ? pal::withAlpha(pal::Danger, 0.7f) : pal::Border);
        ImGui::PushFont(fontRegular(), 15.0f);
        ImGui::SetNextItemWidth(fieldWidth);
        ImGui::InputTextWithHint(id, hint, buffer, size);
        ImGui::PopFont();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        drawFieldOutline(ImGui::IsItemActive(), hasError);
        drawFieldNote(error, helper, fieldWidth);
        ImGui::PopStyleVar();
        ImGui::Dummy(ImVec2(fieldWidth, 16.0f));
    }

    void passwordField(const char *caption, const char *id, char *buffer, std::size_t size, bool &reveal,
                       bool required, const char *error, const char *helper, float width)
    {
        const bool hasError = error && error[0];

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 0.0f));
        bool toggled = false;
        const float fieldWidth = drawCaption(caption, required, reveal ? "Ocultar" : "Mostrar", &toggled, width);
        if (toggled)
            reveal = !reveal;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(14.0f, 11.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, hasError ? pal::withAlpha(pal::Danger, 0.7f) : pal::Border);
        ImGui::PushFont(fontRegular(), 15.0f);
        ImGui::SetNextItemWidth(fieldWidth);
        ImGui::InputText(id, buffer, size, reveal ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_Password);
        ImGui::PopFont();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        drawFieldOutline(ImGui::IsItemActive(), hasError);
        drawFieldNote(error, helper, fieldWidth);
        ImGui::PopStyleVar();
        ImGui::Dummy(ImVec2(fieldWidth, 16.0f));
    }

    bool checkbox(const char *id, const char *labelText, bool *value)
    {
        ImDrawList *dl = ImGui::GetWindowDrawList();
        const float box = 22.0f;
        const float gap = 12.0f;
        const float textSize = 14.5f;
        const float width = ImGui::GetContentRegionAvail().x;
        const float wrap = width - box - gap;

        const ImVec2 textExtent = measure(fontRegular(), textSize, labelText, wrap);
        const float height = std::max(box, textExtent.y);
        const ImVec2 p = ImGui::GetCursorScreenPos();

        ImGui::PushID(id);
        const bool pressed = ImGui::InvisibleButton("##check", ImVec2(width, height));
        const bool hovered = ImGui::IsItemHovered();
        ImGui::PopID();
        if (pressed)
            *value = !*value;
        if (hovered)
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

        const ImVec2 b0(p.x, p.y + (height - box) * 0.5f - (textExtent.y > box ? (textExtent.y - box) * 0.5f : 0.0f));
        const ImVec2 b1(b0.x + box, b0.y + box);

        if (*value)
        {
            dl->AddRectFilled(b0, b1, u32(hovered ? pal::PrimaryHover : pal::Primary), 6.0f);
            drawCheck(dl, ImVec2((b0.x + b1.x) * 0.5f, (b0.y + b1.y) * 0.5f), box * 0.52f, IM_COL32(255, 255, 255, 255), 2.2f);
        }
        else
        {
            dl->AddRectFilled(b0, b1, u32(pal::Input), 6.0f);
            dl->AddRect(b0, b1, u32(hovered ? pal::TextMuted : pal::Border), 6.0f, 0, 1.5f);
        }

        dl->AddText(fontRegular(), textSize, ImVec2(p.x + box + gap, p.y + 2.0f), u32(pal::Text), labelText, nullptr, wrap);
        return pressed;
    }

    // ---- Botones y estados ---------------------------------------------------
    bool primaryButton(const char *text, ImVec2 size, bool enabled)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, enabled ? pal::Primary : pal::withAlpha(pal::Primary, 0.28f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, enabled ? pal::PrimaryHover : pal::withAlpha(pal::Primary, 0.28f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, enabled ? pal::PrimaryDown : pal::withAlpha(pal::Primary, 0.28f));
        ImGui::PushStyleColor(ImGuiCol_Text, enabled ? ImVec4(1, 1, 1, 1) : ImVec4(1, 1, 1, 0.55f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::PushFont(fontSemibold(), 15.0f);

        const bool clicked = ImGui::Button(text, size);

        ImGui::PopFont();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(4);
        if (enabled && ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        return enabled && clicked;
    }

    bool secondaryButton(const char *text, ImVec2 size, bool enabled)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, enabled ? pal::withAlpha(pal::Text, 0.07f) : ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, pal::withAlpha(pal::Text, 0.12f));
        ImGui::PushStyleColor(ImGuiCol_Border, pal::Border);
        ImGui::PushStyleColor(ImGuiCol_Text, enabled ? pal::Text : pal::TextDisabled);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushFont(fontMedium(), 15.0f);

        const bool clicked = ImGui::Button(text, size);

        ImGui::PopFont();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(5);
        if (enabled && ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        return enabled && clicked;
    }

    void badge(const char *text, const ImVec4 &color)
    {
        ImDrawList *dl = ImGui::GetWindowDrawList();
        const float size = 12.5f;
        const ImVec2 extent = measure(fontSemibold(), size, text);
        const float padX = 11.0f;
        const float height = 24.0f;
        const ImVec2 p = ImGui::GetCursorScreenPos();
        const ImVec2 p1(p.x + extent.x + padX * 2.0f, p.y + height);

        dl->AddRectFilled(p, p1, u32(pal::withAlpha(color, 0.14f)), height * 0.5f);
        dl->AddRect(p, p1, u32(pal::withAlpha(color, 0.40f)), height * 0.5f, 0, 1.0f);
        dl->AddText(fontSemibold(), size, ImVec2(p.x + padX, p.y + (height - extent.y) * 0.5f), u32(color), text);
        ImGui::Dummy(ImVec2(p1.x - p.x, height));
    }

    void progressBar(float fraction, float height)
    {
        ImDrawList *dl = ImGui::GetWindowDrawList();
        const float width = ImGui::GetContentRegionAvail().x;
        const ImVec2 p = ImGui::GetCursorScreenPos();
        const float f = std::clamp(fraction, 0.0f, 1.0f);

        dl->AddRectFilled(p, ImVec2(p.x + width, p.y + height), u32(pal::Input), height * 0.5f);
        dl->AddRect(p, ImVec2(p.x + width, p.y + height), u32(pal::Border), height * 0.5f, 0, 1.0f);
        if (f > 0.0f)
        {
            const float fillWidth = std::max(height, width * f);
            dl->AddRectFilled(p, ImVec2(p.x + fillWidth, p.y + height), u32(pal::Primary), height * 0.5f);
        }
        ImGui::Dummy(ImVec2(width, height));
    }

    // ---- Dibujo --------------------------------------------------------------
    void drawLogoMark(ImDrawList *dl, ImVec2 c, float r, ImU32 accent, ImU32 fg)
    {
        dl->AddRectFilled(ImVec2(c.x - r, c.y - r), ImVec2(c.x + r, c.y + r), accent, r * 0.38f);
        dl->AddCircle(c, r * 0.52f, fg, 28, std::max(1.5f, r * 0.12f));
        dl->AddCircleFilled(c, r * 0.24f, fg, 20);
    }

    void drawCheck(ImDrawList *dl, ImVec2 c, float s, ImU32 color, float thickness)
    {
        dl->PathLineTo(ImVec2(c.x - s * 0.42f, c.y + s * 0.02f));
        dl->PathLineTo(ImVec2(c.x - s * 0.10f, c.y + s * 0.34f));
        dl->PathLineTo(ImVec2(c.x + s * 0.46f, c.y - s * 0.30f));
        dl->PathStroke(color, ImDrawFlags_None, thickness);
    }

    void drawSpinner(ImDrawList *dl, ImVec2 c, float radius, ImU32 color, float thickness)
    {
        const float t = static_cast<float>(ImGui::GetTime());
        const float start = t * 5.0f;
        dl->PathClear();
        dl->PathArcTo(c, radius, start, start + kPi * 1.45f, 24);
        dl->PathStroke(color, ImDrawFlags_None, thickness);
    }

    void drawIcon(ImDrawList *dl, Icon icon, ImVec2 c, float s, ImU32 col, float th)
    {
        switch (icon)
        {
        case Icon::Building:
        {
            const ImVec2 a(c.x - s * 0.30f, c.y - s * 0.42f);
            const ImVec2 b(c.x + s * 0.30f, c.y + s * 0.42f);
            dl->AddRect(a, b, col, s * 0.06f, 0, th);
            for (int row = 0; row < 3; ++row)
                for (int colIdx = 0; colIdx < 2; ++colIdx)
                {
                    const float x = c.x - s * 0.17f + colIdx * s * 0.22f;
                    const float y = c.y - s * 0.30f + row * s * 0.20f;
                    dl->AddRectFilled(ImVec2(x, y), ImVec2(x + s * 0.10f, y + s * 0.10f), col, 1.5f);
                }
            dl->AddLine(ImVec2(c.x - s * 0.08f, c.y + s * 0.42f), ImVec2(c.x - s * 0.08f, c.y + s * 0.24f), col, th);
            dl->AddLine(ImVec2(c.x + s * 0.08f, c.y + s * 0.42f), ImVec2(c.x + s * 0.08f, c.y + s * 0.24f), col, th);
            dl->AddLine(ImVec2(c.x - s * 0.08f, c.y + s * 0.24f), ImVec2(c.x + s * 0.08f, c.y + s * 0.24f), col, th);
            break;
        }
        case Icon::User:
        {
            dl->AddCircle(ImVec2(c.x, c.y - s * 0.16f), s * 0.17f, col, 24, th);
            dl->PathClear();
            dl->PathArcTo(ImVec2(c.x, c.y + s * 0.44f), s * 0.34f, kPi * 1.08f, kPi * 1.92f, 20);
            dl->PathStroke(col, ImDrawFlags_None, th);
            break;
        }
        case Icon::Shield:
        {
            dl->PathClear();
            dl->PathLineTo(ImVec2(c.x, c.y - s * 0.44f));
            dl->PathLineTo(ImVec2(c.x + s * 0.34f, c.y - s * 0.30f));
            dl->PathLineTo(ImVec2(c.x + s * 0.34f, c.y + s * 0.04f));
            dl->PathBezierCubicCurveTo(ImVec2(c.x + s * 0.34f, c.y + s * 0.26f), ImVec2(c.x + s * 0.14f, c.y + s * 0.38f), ImVec2(c.x, c.y + s * 0.46f));
            dl->PathBezierCubicCurveTo(ImVec2(c.x - s * 0.14f, c.y + s * 0.38f), ImVec2(c.x - s * 0.34f, c.y + s * 0.26f), ImVec2(c.x - s * 0.34f, c.y + s * 0.04f));
            dl->PathLineTo(ImVec2(c.x - s * 0.34f, c.y - s * 0.30f));
            dl->PathStroke(col, ImDrawFlags_Closed, th);
            drawCheck(dl, ImVec2(c.x, c.y + s * 0.02f), s * 0.36f, col, th);
            break;
        }
        case Icon::Database:
        {
            const float rx = s * 0.32f;
            const float ry = s * 0.11f;
            const float top = c.y - s * 0.28f;
            const float bottom = c.y + s * 0.28f;
            dl->AddEllipse(ImVec2(c.x, top), ImVec2(rx, ry), col, 0.0f, 28, th);
            dl->AddLine(ImVec2(c.x - rx, top), ImVec2(c.x - rx, bottom), col, th);
            dl->AddLine(ImVec2(c.x + rx, top), ImVec2(c.x + rx, bottom), col, th);
            for (int i = 0; i < 2; ++i)
            {
                const float y = (i == 0) ? c.y : bottom;
                dl->PathClear();
                dl->PathArcTo(ImVec2(c.x, y), rx, 0.0f, kPi, 20); // el eje Y de pantalla va hacia abajo
                // PathArcTo dibuja con ry = rx; se aplana escalando los puntos.
                for (int k = 0; k < dl->_Path.Size; ++k)
                    dl->_Path[k].y = y + (dl->_Path[k].y - y) * (ry / rx);
                dl->PathStroke(col, ImDrawFlags_None, th);
            }
            break;
        }
        case Icon::Install:
        {
            dl->AddLine(ImVec2(c.x, c.y - s * 0.40f), ImVec2(c.x, c.y + s * 0.18f), col, th);
            dl->PathClear();
            dl->PathLineTo(ImVec2(c.x - s * 0.20f, c.y - s * 0.02f));
            dl->PathLineTo(ImVec2(c.x, c.y + s * 0.20f));
            dl->PathLineTo(ImVec2(c.x + s * 0.20f, c.y - s * 0.02f));
            dl->PathStroke(col, ImDrawFlags_None, th);
            dl->PathClear();
            dl->PathLineTo(ImVec2(c.x - s * 0.36f, c.y + s * 0.14f));
            dl->PathLineTo(ImVec2(c.x - s * 0.36f, c.y + s * 0.38f));
            dl->PathLineTo(ImVec2(c.x + s * 0.36f, c.y + s * 0.38f));
            dl->PathLineTo(ImVec2(c.x + s * 0.36f, c.y + s * 0.14f));
            dl->PathStroke(col, ImDrawFlags_None, th);
            break;
        }
        case Icon::Check:
            drawCheck(dl, c, s, col, th);
            break;
        }
    }

    void drawStepMarker(ImDrawList *dl, ImVec2 c, float r, StepState state, int number)
    {
        char digits[8];
        std::snprintf(digits, sizeof(digits), "%d", number);

        switch (state)
        {
        case StepState::Done:
            dl->AddCircleFilled(c, r, u32(pal::withAlpha(pal::Success, 0.22f)), 28);
            dl->AddCircle(c, r, u32(pal::withAlpha(pal::Success, 0.80f)), 28, 1.5f);
            drawCheck(dl, c, r * 0.95f, u32(pal::Success), 2.0f);
            break;
        case StepState::Active:
        {
            dl->AddCircleFilled(c, r, u32(pal::Primary), 28);
            const ImVec2 e = measure(fontSemibold(), 13.0f, digits);
            dl->AddText(fontSemibold(), 13.0f, ImVec2(c.x - e.x * 0.5f, c.y - e.y * 0.5f), IM_COL32(255, 255, 255, 255), digits);
            break;
        }
        case StepState::Pending:
        {
            dl->AddCircle(c, r, u32(pal::Border), 28, 1.5f);
            const ImVec2 e = measure(fontMedium(), 13.0f, digits);
            dl->AddText(fontMedium(), 13.0f, ImVec2(c.x - e.x * 0.5f, c.y - e.y * 0.5f), u32(pal::TextMuted), digits);
            break;
        }
        }
    }

    int passwordStrength(const char *password)
    {
        const std::size_t length = std::strlen(password);
        if (length == 0)
            return 0;

        bool lower = false, upper = false, digit = false, symbol = false;
        for (std::size_t i = 0; i < length; ++i)
        {
            const unsigned char ch = static_cast<unsigned char>(password[i]);
            if (ch >= 'a' && ch <= 'z')
                lower = true;
            else if (ch >= 'A' && ch <= 'Z')
                upper = true;
            else if (ch >= '0' && ch <= '9')
                digit = true;
            else
                symbol = true;
        }

        int score = 0;
        if (length >= 8)
            ++score;
        if (length >= 12)
            ++score;
        if (lower && upper)
            ++score;
        if (digit)
            ++score;
        if (symbol)
            ++score;

        if (length < 8)
            return 1;
        return std::clamp(score, 1, 4);
    }
}
