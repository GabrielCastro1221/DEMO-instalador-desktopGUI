#include "AdminSetupView.h"

#include "UiKit.h"
#include "core/styles/ColorPalette.h"
#include "core/styles/Fonts.h"

#include <imgui.h>

#include <cstring>

namespace pal = installer::styles;
namespace ui = installer::ui;

bool AdminSetupView::valid() const
{
    return name_[0] && user_[0] && std::strlen(pass_) >= 8 && std::strcmp(pass_, confirm_) == 0;
}

void AdminSetupView::render()
{
    const bool tooShort = pass_[0] != '\0' && std::strlen(pass_) < 8;
    const bool mismatch = confirm_[0] != '\0' && std::strcmp(pass_, confirm_) != 0;

    ui::cardBegin("##admin_form", ImVec2(0.0f, 0.0f));
    {
        ui::label(pal::Fonts().semibold, 16.0f, pal::Text, "Datos de la cuenta");
        ImGui::Dummy(ImVec2(0.0f, 6.0f));

        const float inner = ImGui::GetContentRegionAvail().x;
        const float columnGap = 16.0f;
        const float column = (inner - columnGap) * 0.5f;

        ImGui::BeginGroup();
        ui::field("Nombre completo", "##adminname", name_, sizeof(name_), "Ej. María Fernanda Gómez", true,
                  nullptr, nullptr, column);
        ImGui::EndGroup();
        ImGui::SameLine(0.0f, columnGap);
        ImGui::BeginGroup();
        ui::field("Usuario", "##adminuser", user_, sizeof(user_), "admin", true,
                  user_[0] == '\0' ? "El usuario es obligatorio." : nullptr, "Con este usuario iniciarás sesión.", column);
        ImGui::EndGroup();

        ImGui::BeginGroup();
        ui::passwordField("Contraseña", "##adminpass", pass_, sizeof(pass_), revealPass_, true,
                          tooShort ? "Debe tener al menos 8 caracteres." : nullptr, nullptr, column);
        ImGui::EndGroup();
        ImGui::SameLine(0.0f, columnGap);
        ImGui::BeginGroup();
        ui::passwordField("Confirmar contraseña", "##adminconfirm", confirm_, sizeof(confirm_), revealConfirm_, true,
                          mismatch ? "Las contraseñas no coinciden." : nullptr, nullptr, column);
        ImGui::EndGroup();

        // Medidor de seguridad
        {
            const int level = ui::passwordStrength(pass_);
            const char *names[] = {"", "Débil", "Aceptable", "Buena", "Fuerte"};
            const ImVec4 colors[] = {pal::Border, pal::Danger, pal::Warning, pal::Primary, pal::Success};

            ImDrawList *dl = ImGui::GetWindowDrawList();
            const ImVec2 p = ImGui::GetCursorScreenPos();
            const float labelWidth = 84.0f;
            const float barsWidth = inner - labelWidth - 12.0f;
            const float segGap = 6.0f;
            const float seg = (barsWidth - segGap * 3.0f) / 4.0f;

            for (int i = 0; i < 4; ++i)
            {
                const float x0 = p.x + i * (seg + segGap);
                const bool on = i < level;
                dl->AddRectFilled(ImVec2(x0, p.y + 4.0f), ImVec2(x0 + seg, p.y + 9.0f),
                                  ui::u32(on ? colors[level] : pal::withAlpha(pal::Border, 0.55f)), 3.0f);
            }
            if (level > 0)
                dl->AddText(pal::Fonts().medium, 13.0f, ImVec2(p.x + barsWidth + 12.0f, p.y), ui::u32(colors[level]), names[level]);
            ImGui::Dummy(ImVec2(inner, 18.0f));

            ui::paragraph(pal::Fonts().regular, 12.5f, pal::TextMuted,
                          "Usa 12 o más caracteres e incluye mayúsculas, números y símbolos para una contraseña más segura.", inner);
        }
    }
    ui::cardEnd();

    ImGui::Dummy(ImVec2(0.0f, 2.0f));
    ui::callout(ui::Kind::Info, "Permisos de administrador",
                "Esta cuenta tendrá acceso total al sistema: ventas, inventario, reportes, usuarios y configuración. "
                "Podrás crear más usuarios con permisos limitados desde el módulo Personal.");
}
