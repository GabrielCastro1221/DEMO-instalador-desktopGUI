#include "InstallProgressView.h"

#include "UiKit.h"
#include "core/styles/ColorPalette.h"
#include "core/styles/Fonts.h"

#include <imgui.h>

#include <algorithm>
#include <cstdio>

namespace pal = installer::styles;
namespace ui = installer::ui;

void InstallProgressView::start()
{
    progress_ = 0;
    finished_ = false;
    last_ = ImGui::GetTime();
}

void InstallProgressView::render(bool started)
{
    if (started && !finished_ && ImGui::GetTime() - last_ > .06)
    {
        progress_ += .007f;
        last_ = ImGui::GetTime();
        if (progress_ >= 1)
        {
            progress_ = 1;
            finished_ = true;
        }
    }

    static const char *tasks[] = {"Validando requisitos", "Preparando archivos", "Configurando base de datos",
                                  "Creando administrador", "Guardando configuración", "Finalizando"};
    constexpr int kTasks = 6;
    const int current = std::min(kTasks - 1, static_cast<int>(progress_ * kTasks));

    ui::cardBegin("##progress_card", ImVec2(0.0f, 0.0f));
    {
        ImDrawList *dl = ImGui::GetWindowDrawList();

        char percent[16];
        std::snprintf(percent, sizeof(percent), "%d%%", static_cast<int>(progress_ * 100.0f));

        const float width = ImGui::GetContentRegionAvail().x;
        const ImVec2 head = ImGui::GetCursorScreenPos();
        dl->AddText(pal::Fonts().semibold, 16.0f, head, ui::u32(pal::Text), finished_ ? "Instalación completada" : tasks[current]);
        const ImVec2 pe = ui::measure(pal::Fonts().semibold, 22.0f, percent);
        dl->AddText(pal::Fonts().semibold, 22.0f, ImVec2(head.x + width - pe.x, head.y - 3.0f), ui::u32(pal::Text), percent);
        ImGui::Dummy(ImVec2(width, 28.0f));

        ui::progressBar(progress_, 12.0f);
        ImGui::Dummy(ImVec2(0.0f, 14.0f));

        for (int i = 0; i < kTasks; ++i)
        {
            const bool done = i < current || finished_;
            const bool active = i == current && !finished_;

            const ImVec2 p = ImGui::GetCursorScreenPos();
            const float rowH = 44.0f;
            const ImVec2 center(p.x + 14.0f, p.y + rowH * 0.5f);

            if (active)
                dl->AddRectFilled(ImVec2(p.x - 10.0f, p.y + 2.0f), ImVec2(p.x + width + 10.0f, p.y + rowH - 2.0f),
                                  ui::u32(pal::withAlpha(pal::Primary, 0.09f)), 9.0f);

            if (done)
            {
                dl->AddCircleFilled(center, 11.0f, ui::u32(pal::withAlpha(pal::Success, 0.22f)), 24);
                dl->AddCircle(center, 11.0f, ui::u32(pal::withAlpha(pal::Success, 0.80f)), 24, 1.5f);
                ui::drawCheck(dl, center, 13.0f, ui::u32(pal::Success), 2.0f);
            }
            else if (active)
            {
                dl->AddCircle(center, 11.0f, ui::u32(pal::withAlpha(pal::Primary, 0.25f)), 24, 1.5f);
                ui::drawSpinner(dl, center, 11.0f, ui::u32(pal::Primary), 2.4f);
            }
            else
            {
                dl->AddCircle(center, 11.0f, ui::u32(pal::Border), 24, 1.5f);
            }

            const ImVec4 textColor = done ? pal::Text : (active ? pal::Text : pal::TextDisabled);
            dl->AddText(done || active ? pal::Fonts().medium : pal::Fonts().regular, 15.0f,
                        ImVec2(p.x + 38.0f, p.y + rowH * 0.5f - 9.0f), ui::u32(textColor), tasks[i]);

            const char *status = done ? "Completado" : (active ? "En curso" : "Pendiente");
            const ImVec4 statusColor = done ? pal::Success : (active ? pal::Primary : pal::TextDisabled);
            const ImVec2 se = ui::measure(pal::Fonts().medium, 13.0f, status);
            dl->AddText(pal::Fonts().medium, 13.0f, ImVec2(p.x + width - se.x, p.y + rowH * 0.5f - 8.0f), ui::u32(statusColor), status);

            ImGui::Dummy(ImVec2(width, rowH));
        }
    }
    ui::cardEnd();

    ImGui::Dummy(ImVec2(0.0f, 2.0f));
    ui::callout(ui::Kind::Warning, "Progreso simulado",
                "Esta versión es una demostración visual: no se copian archivos ni se modifica MySQL.");
}
