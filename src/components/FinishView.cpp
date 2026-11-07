#include "FinishView.h"

#include "UiKit.h"
#include "core/styles/ColorPalette.h"
#include "core/styles/Fonts.h"

#include <imgui.h>

#include <cstdio>

namespace pal = installer::styles;
namespace ui = installer::ui;

void FinishView::render(bool &launch, const InstallSummary &summary)
{
    ui::cardBegin("##finish_hero", ImVec2(0.0f, 0.0f), 28.0f, 26.0f);
    {
        ImDrawList *dl = ImGui::GetWindowDrawList();
        const ImVec2 p = ImGui::GetCursorScreenPos();
        const float width = ImGui::GetContentRegionAvail().x;
        const ImVec2 c(p.x + 36.0f, p.y + 36.0f);

        dl->AddCircleFilled(c, 36.0f, ui::u32(pal::withAlpha(pal::Success, 0.16f)), 40);
        dl->AddCircle(c, 36.0f, ui::u32(pal::withAlpha(pal::Success, 0.55f)), 40, 1.5f);
        dl->AddCircleFilled(c, 25.0f, ui::u32(pal::Success), 40);
        ui::drawCheck(dl, c, 28.0f, IM_COL32(255, 255, 255, 255), 3.2f);

        dl->AddText(pal::Fonts().semibold, 24.0f, ImVec2(p.x + 92.0f, p.y + 6.0f), ui::u32(pal::Text), "¡Asistente completado!");
        const float wrap = width - 92.0f;
        const char *message = "El recorrido de demostración terminó. No se instalaron archivos ni se modificó MySQL.";
        dl->AddText(pal::Fonts().regular, 14.5f, ImVec2(p.x + 92.0f, p.y + 40.0f), ui::u32(pal::TextMuted), message, nullptr, wrap);
        ImGui::Dummy(ImVec2(width, 82.0f));
    }
    ui::cardEnd();

    ImGui::Dummy(ImVec2(0.0f, 4.0f));

    ui::cardBegin("##finish_summary", ImVec2(0.0f, 0.0f));
    {
        ui::label(pal::Fonts().semibold, 16.0f, pal::Text, "Resumen de la configuración");
        ImGui::Dummy(ImVec2(0.0f, 8.0f));

        ImDrawList *dl = ImGui::GetWindowDrawList();
        const float width = ImGui::GetContentRegionAvail().x;

        auto row = [&](const char *caption, const char *value, bool last)
        {
            const ImVec2 p = ImGui::GetCursorScreenPos();
            dl->AddText(pal::Fonts().regular, 14.0f, ImVec2(p.x, p.y + 11.0f), ui::u32(pal::TextMuted), caption);
            const ImVec2 e = ui::measure(pal::Fonts().medium, 14.0f, value);
            dl->AddText(pal::Fonts().medium, 14.0f, ImVec2(p.x + width - e.x, p.y + 11.0f), ui::u32(pal::Text), value);
            if (!last)
                dl->AddLine(ImVec2(p.x, p.y + 38.0f), ImVec2(p.x + width, p.y + 38.0f), ui::u32(pal::Separator), 1.0f);
            ImGui::Dummy(ImVec2(width, 38.0f));
        };

        char database[200];
        std::snprintf(database, sizeof(database), "%s:%s / %s", summary.dbHost, summary.dbPort, summary.dbName);
        char admin[220];
        if (summary.adminName[0])
            std::snprintf(admin, sizeof(admin), "%s (%s)", summary.adminName, summary.adminUser);
        else
            std::snprintf(admin, sizeof(admin), "%s", summary.adminUser);

        row("Empresa", summary.company, false);
        row("Ciudad", summary.city[0] ? summary.city : "—", false);
        row("Administrador", admin, false);
        row("Base de datos", database, true);
    }
    ui::cardEnd();

    ImGui::Dummy(ImVec2(0.0f, 4.0f));
    ui::checkbox("##launch", "Iniciar Miscelánea Bellavista POS al finalizar", &launch);
    ui::paragraph(pal::Fonts().regular, 12.5f, pal::TextMuted,
                  "En la versión funcional, esta opción abrirá el ejecutable instalado.", 0.0f);
}
