#include "DatabaseSetupView.h"

#include "UiKit.h"
#include "core/styles/ColorPalette.h"
#include "core/styles/Fonts.h"

#include <imgui.h>

#include <cctype>

namespace pal = installer::styles;
namespace ui = installer::ui;

namespace
{
    bool portIsNumeric(const char *port)
    {
        if (!port[0])
            return false;
        for (const char *c = port; *c; ++c)
            if (!std::isdigit(static_cast<unsigned char>(*c)))
                return false;
        return true;
    }
}

bool DatabaseSetupView::valid() const
{
    return host_[0] && portIsNumeric(port_) && db_[0] && user_[0];
}

void DatabaseSetupView::render()
{
    ui::cardBegin("##db_form", ImVec2(0.0f, 0.0f));
    {
        ui::label(pal::Fonts().semibold, 16.0f, pal::Text, "Conexión con MySQL");
        ImGui::Dummy(ImVec2(0.0f, 8.0f));

        ui::checkbox("##local", "MySQL está instalado en este equipo", &local_);
        ImGui::Dummy(ImVec2(0.0f, 6.0f));

        const float inner = ImGui::GetContentRegionAvail().x;
        const float columnGap = 16.0f;
        const float portWidth = 150.0f;
        const float hostWidth = inner - portWidth - columnGap;
        const float column = (inner - columnGap) * 0.5f;

        ImGui::BeginGroup();
        ui::field("Servidor / host", "##host", host_, sizeof(host_), "127.0.0.1", true,
                  host_[0] == '\0' ? "Indica el servidor." : nullptr, nullptr, hostWidth);
        ImGui::EndGroup();
        ImGui::SameLine(0.0f, columnGap);
        ImGui::BeginGroup();
        ui::field("Puerto", "##port", port_, sizeof(port_), "3306", true,
                  portIsNumeric(port_) ? nullptr : "Solo números.", nullptr, portWidth);
        ImGui::EndGroup();

        ui::field("Base de datos", "##dbname", db_, sizeof(db_), "bellavista_pos", true,
                  db_[0] == '\0' ? "Indica el nombre de la base de datos." : nullptr);

        ImGui::BeginGroup();
        ui::field("Usuario de conexión", "##dbuser", user_, sizeof(user_), "bellavista_app", true,
                  user_[0] == '\0' ? "Indica el usuario." : nullptr, nullptr, column);
        ImGui::EndGroup();
        ImGui::SameLine(0.0f, columnGap);
        ImGui::BeginGroup();
        ui::passwordField("Contraseña", "##dbpass", pass_, sizeof(pass_), revealPass_, false, nullptr, nullptr, column);
        ImGui::EndGroup();

        ImGui::Dummy(ImVec2(0.0f, 4.0f));

        const double now = ImGui::GetTime();
        const bool testing = testStartedAt_ >= 0.0 && !testDone_;
        if (testing && now - testStartedAt_ > 1.1)
            testDone_ = true;

        if (ui::secondaryButton(testing ? "Probando..." : "Probar conexión", ImVec2(168.0f, 40.0f), !testing))
        {
            testStartedAt_ = now;
            testDone_ = false;
        }

        ImGui::SameLine(0.0f, 14.0f);
        const ImVec2 p = ImGui::GetCursorScreenPos();
        if (testing)
        {
            ui::drawSpinner(ImGui::GetWindowDrawList(), ImVec2(p.x + 10.0f, p.y + 20.0f), 8.0f, ui::u32(pal::Primary));
            ImGui::SetCursorScreenPos(ImVec2(p.x + 28.0f, p.y + 10.0f));
            ui::label(pal::Fonts().regular, 14.0f, pal::TextMuted, "Comprobando credenciales...");
        }
        else if (testDone_)
        {
            ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + 8.0f));
            ui::badge("Modo demostración: no se realiza la conexión real", pal::Warning);
        }
        else
        {
            ImGui::Dummy(ImVec2(1.0f, 40.0f));
        }
    }
    ui::cardEnd();

    ImGui::Dummy(ImVec2(0.0f, 2.0f));
    ui::callout(ui::Kind::Info, "Recomendación de seguridad",
                "Usa un usuario de MySQL dedicado para esta aplicación, con permisos solo sobre la base de datos del sistema. "
                "Evita utilizar el usuario root.");
}
