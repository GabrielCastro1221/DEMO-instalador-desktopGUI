#include "CompanySetupView.h"

#include "UiKit.h"
#include "core/styles/ColorPalette.h"
#include "core/styles/Fonts.h"

#include <imgui.h>

#include <algorithm>
#include <cstdio>
#include <cstring>

namespace pal = installer::styles;
namespace ui = installer::ui;

bool CompanySetupView::emailLooksValid() const
{
    if (email_[0] == '\0')
        return true;
    const char *at = std::strchr(email_, '@');
    return at && at != email_ && std::strchr(at, '.') && email_[std::strlen(email_) - 1] != '.';
}

bool CompanySetupView::valid() const
{
    return trade_[0] != '\0' && emailLooksValid();
}

void CompanySetupView::render()
{
    const float avail = ImGui::GetContentRegionAvail().x;
    const bool showPreview = avail >= 800.0f;
    const float gap = 20.0f;
    const float previewWidth = 300.0f;
    const float formWidth = showPreview ? avail - previewWidth - gap : avail;

    ui::cardBegin("##company_form", ImVec2(formWidth, 0.0f));
    {
        ui::label(pal::Fonts().semibold, 16.0f, pal::Text, "Información general");
        ImGui::Dummy(ImVec2(0.0f, 6.0f));

        const float inner = ImGui::GetContentRegionAvail().x;
        const float columnGap = 16.0f;
        const float column = (inner - columnGap) * 0.5f;

        ImGui::BeginGroup();
        ui::field("Nombre comercial", "##trade", trade_, sizeof(trade_), "Nombre que verán tus clientes", true,
                  trade_[0] == '\0' ? "El nombre comercial es obligatorio." : nullptr, nullptr, column);
        ImGui::EndGroup();
        ImGui::SameLine(0.0f, columnGap);
        ImGui::BeginGroup();
        ui::field("Razón social", "##legal", legal_, sizeof(legal_), "Nombre legal de la empresa", false, nullptr, nullptr, column);
        ImGui::EndGroup();

        ImGui::BeginGroup();
        ui::field("Tipo de documento", "##doctype", type_, sizeof(type_), "NIT", false, nullptr, nullptr, column);
        ImGui::EndGroup();
        ImGui::SameLine(0.0f, columnGap);
        ImGui::BeginGroup();
        ui::field("Número de documento", "##docnumber", number_, sizeof(number_), "Ej. 900.123.456-7", false, nullptr, nullptr, column);
        ImGui::EndGroup();

        ImGui::BeginGroup();
        ui::field("Teléfono", "##phone", phone_, sizeof(phone_), "Ej. 300 123 4567", false, nullptr, nullptr, column);
        ImGui::EndGroup();
        ImGui::SameLine(0.0f, columnGap);
        ImGui::BeginGroup();
        ui::field("Correo electrónico", "##email", email_, sizeof(email_), "contacto@tuempresa.com", false,
                  emailLooksValid() ? nullptr : "Escribe un correo válido.", nullptr, column);
        ImGui::EndGroup();

        ImGui::BeginGroup();
        ui::field("Ciudad", "##city", city_, sizeof(city_), "Ciudad", false, nullptr, nullptr, column);
        ImGui::EndGroup();
        ImGui::SameLine(0.0f, columnGap);
        ImGui::BeginGroup();
        ui::field("Moneda", "##currency", currency_, sizeof(currency_), "COP - Peso colombiano", false, nullptr, nullptr, column);
        ImGui::EndGroup();

        ui::field("Dirección", "##address", address_, sizeof(address_), "Calle, número y barrio");
    }
    ui::cardEnd();

    if (showPreview)
    {
        ImGui::SameLine(0.0f, gap);
        ui::cardBegin("##company_preview", ImVec2(previewWidth, 330.0f));
        {
            ui::sectionLabel("VISTA PREVIA");
            ImGui::Dummy(ImVec2(0.0f, 8.0f));

            ImDrawList *dl = ImGui::GetWindowDrawList();
            const ImVec2 p = ImGui::GetCursorScreenPos();
            const float w = ImGui::GetContentRegionAvail().x;
            const float boxH = 76.0f;

            dl->AddRectFilled(p, ImVec2(p.x + w, p.y + boxH), ui::u32(pal::Background), 10.0f);
            dl->AddRect(p, ImVec2(p.x + w, p.y + boxH), ui::u32(pal::Border), 10.0f);
            ui::drawLogoMark(dl, ImVec2(p.x + 34.0f, p.y + boxH * 0.5f), 17.0f, ui::u32(pal::Accent), ui::u32(pal::Text));

            const char *name = trade_[0] ? trade_ : "Nombre comercial";
            dl->PushClipRect(ImVec2(p.x + 62.0f, p.y), ImVec2(p.x + w - 8.0f, p.y + boxH), true);
            dl->AddText(pal::Fonts().semibold, 15.0f, ImVec2(p.x + 62.0f, p.y + boxH * 0.5f - 18.0f), ui::u32(pal::Text), name);
            dl->AddText(pal::Fonts().regular, 13.0f, ImVec2(p.x + 62.0f, p.y + boxH * 0.5f + 3.0f), ui::u32(pal::TextMuted), "Punto de venta");
            dl->PopClipRect();
            ImGui::Dummy(ImVec2(w, boxH + 14.0f));

            auto row = [&](const char *caption, const char *value)
            {
                ui::label(pal::Fonts().medium, 12.0f, pal::TextDisabled, caption);
                ui::label(pal::Fonts().regular, 14.0f, value[0] ? pal::Text : pal::TextDisabled, value[0] ? value : "—");
                ImGui::Dummy(ImVec2(0.0f, 4.0f));
            };

            char document[96];
            std::snprintf(document, sizeof(document), "%s %s", type_, number_);
            row("RAZÓN SOCIAL", legal_);
            row("DOCUMENTO", number_[0] ? document : "");
            row("CIUDAD", city_);
        }
        ui::cardEnd();
    }
}
