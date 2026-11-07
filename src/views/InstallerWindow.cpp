#include "InstallerWindow.h"

#include "LegalText.h"
#include "components/UiKit.h"
#include "core/styles/ColorPalette.h"
#include "core/styles/Fonts.h"

#include <imgui.h>

#include <algorithm>
#include <cstdio>

namespace
{
    namespace pal = installer::styles;
    namespace ui = installer::ui;

    constexpr int kStepCount = 8;
    constexpr float kSidebarWidth = 268.0f;
    constexpr float kSidebarHeaderHeight = 84.0f;
    constexpr float kFooterHeight = 76.0f;
    constexpr float kContentPadX = 40.0f;
    constexpr float kHeaderHeight = 104.0f;
    constexpr float kMaxContentWidth = 980.0f;

    struct StepInfo
    {
        const char *label;    // texto en la barra lateral
        const char *title;    // título de la pantalla
        const char *subtitle; // frase explicativa
    };

    const StepInfo kSteps[kStepCount] = {
        {"Bienvenida", "Asistente de instalación", "Prepara el sistema para tu negocio en unos pocos pasos."},
        {"Términos de uso", "Términos y condiciones", "Lee las condiciones que regulan el uso del software antes de continuar."},
        {"Privacidad", "Política de privacidad", "Conoce qué información se trata, para qué se utiliza y cómo ejercer tus derechos."},
        {"Empresa", "Datos de la empresa", "Estos datos se mostrarán en la aplicación y en los comprobantes."},
        {"Administrador", "Cuenta administradora", "Crea la cuenta inicial con permisos administrativos."},
        {"Base de datos", "Configuración de MySQL", "Indica cómo se conectará el sistema con la base de datos."},
        {"Instalación", "Instalando el sistema", "Estamos preparando todo. Esto tomará unos instantes."},
        {"Finalizar", "Todo listo", "La configuración inicial ha terminado."},
    };

    struct SectionInfo
    {
        const char *title;
        int first;
        int count;
    };

    const SectionInfo kSections[3] = {
        {"INTRODUCCIÓN", 0, 3},
        {"CONFIGURACIÓN", 3, 3},
        {"INSTALACIÓN", 6, 2},
    };

    int indexOf(InstallerStep step) { return static_cast<int>(step); }

    bool isFormStep(InstallerStep step)
    {
        return step == InstallerStep::Company || step == InstallerStep::Admin || step == InstallerStep::Database;
    }

    // Tarjeta con desplazamiento para los documentos legales.
    void documentBegin(const char *id, ImVec2 size)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 12.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(28.0f, 24.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 4.0f));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, pal::Card);
        ImGui::PushStyleColor(ImGuiCol_Border, pal::Border);
        ImGui::BeginChild(id, size, ImGuiChildFlags_Borders, ImGuiWindowFlags_None);
    }

    void documentEnd()
    {
        ImGui::EndChild();
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(4);
    }
}

// ---------------------------------------------------------------------------
void InstallerWindow::render()
{
    const ImVec2 display = ImGui::GetIO().DisplaySize;
    if (display.x <= 0.0f || display.y <= 0.0f)
        return;

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(display);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, pal::Background);

    const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                   ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::Begin("##Installer", nullptr, flags);

    const float width = ImGui::GetWindowWidth();
    const float height = ImGui::GetWindowHeight();
    const float sidebarWidth = width < 1040.0f ? 244.0f : kSidebarWidth;
    const float left = sidebarWidth + kContentPadX;
    const float contentWidth = std::min(kMaxContentWidth, width - left - kContentPadX);

    drawSidebar(height, sidebarWidth);
    drawHeader(left, contentWidth);

    // Cuerpo desplazable
    const float bodyTop = kHeaderHeight + 8.0f;
    const float bodyHeight = std::max(80.0f, height - bodyTop - kFooterHeight - 12.0f);

    ImGui::SetCursorPos(ImVec2(left, bodyTop));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
    ImGui::BeginChild("##Body", ImVec2(contentWidth, bodyHeight), ImGuiChildFlags_None, ImGuiWindowFlags_None);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 14.0f));

    // Se deja un margen derecho para que la barra de desplazamiento no tape el contenido.
    ImGui::PushItemWidth(contentWidth);
    ImGui::BeginGroup();
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + contentWidth);
    ImGui::Dummy(ImVec2(contentWidth, 0.0f));
    renderBody();
    ImGui::PopTextWrapPos();
    ImGui::EndGroup();
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);

    drawFooter(left, contentWidth, height - kFooterHeight, kFooterHeight);

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);

    if (step_ == InstallerStep::Progress && progress_.finished())
        step_ = InstallerStep::Finish;
}

// ---------------------------------------------------------------------------
void InstallerWindow::drawSidebar(float height, float sidebarWidth)
{
    ImDrawList *dl = ImGui::GetWindowDrawList();
    const ImVec2 origin = ImGui::GetWindowPos();

    // Fondo y borde derecho (igual que la barra lateral del POS)
    dl->AddRectFilled(origin, ImVec2(origin.x + sidebarWidth, origin.y + height), ui::u32(pal::Background));
    dl->AddRectFilled(origin, ImVec2(origin.x + sidebarWidth, origin.y + height), IM_COL32(255, 255, 255, 6));
    dl->AddLine(ImVec2(origin.x + sidebarWidth - 0.5f, origin.y), ImVec2(origin.x + sidebarWidth - 0.5f, origin.y + height),
                IM_COL32(255, 255, 255, 18), 1.0f);

    const float padX = 12.0f;
    const float x = origin.x + padX;
    const float rowW = sidebarWidth - padX * 2.0f;

    // Cabecera con la marca
    {
        const float cy = origin.y + kSidebarHeaderHeight * 0.5f;
        ui::drawLogoMark(dl, ImVec2(x + 26.0f, cy), 17.0f, ui::u32(pal::Accent), ui::u32(pal::Text));
        dl->AddText(pal::Fonts().semibold, 15.0f, ImVec2(x + 54.0f, cy - 17.0f), ui::u32(pal::Text), "Miscelánea Bellavista");
        dl->AddText(pal::Fonts().regular, 13.0f, ImVec2(x + 54.0f, cy + 3.0f), ui::u32(pal::TextMuted), "Asistente de instalación");
    }

    const float footerBlock = 74.0f;
    const float available = height - kSidebarHeaderHeight - footerBlock - 3 * 30.0f;
    const float rowH = std::clamp(available / kStepCount, 36.0f, 44.0f);

    const int current = indexOf(step_);
    const bool canNavigate = current < indexOf(InstallerStep::Progress);

    float y = origin.y + kSidebarHeaderHeight;
    for (const SectionInfo &section : kSections)
    {
        dl->AddText(pal::Fonts().medium, 12.0f, ImVec2(x + 10.0f, y + 9.0f), ui::u32(ImVec4(0.47f, 0.51f, 0.58f, 1.0f)), section.title);
        y += 30.0f;

        for (int i = section.first; i < section.first + section.count; ++i)
        {
            const bool active = i == current;
            const bool done = i < current;
            const bool clickable = done && canNavigate;

            ImGui::SetCursorScreenPos(ImVec2(x, y));
            char id[24];
            std::snprintf(id, sizeof(id), "##step%d", i);
            ImGui::InvisibleButton(id, ImVec2(rowW, rowH));
            const bool hovered = ImGui::IsItemHovered() && clickable;
            if (hovered)
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (clickable && ImGui::IsItemClicked())
                goToStep(static_cast<InstallerStep>(i));

            const ImVec2 p0(x, y);
            const ImVec2 p1(x + rowW, y + rowH);
            if (active)
            {
                dl->AddRectFilled(p0, p1, ui::u32(pal::withAlpha(pal::Accent, 0.20f)), 10.0f);
                dl->AddRectFilled(ImVec2(origin.x, y + 9.0f), ImVec2(origin.x + 3.0f, y + rowH - 9.0f), ui::u32(pal::Accent), 2.0f);
            }
            else if (hovered)
            {
                dl->AddRectFilled(p0, p1, IM_COL32(255, 255, 255, 14), 10.0f);
            }

            const float cy = y + rowH * 0.5f;
            ui::drawStepMarker(dl, ImVec2(x + 26.0f, cy), 11.0f,
                               active ? ui::StepState::Active : (done ? ui::StepState::Done : ui::StepState::Pending), i + 1);

            const ImVec4 textColor = (active || hovered) ? pal::Text : (done ? pal::Text : pal::TextMuted);
            ImFont *font = active ? pal::Fonts().semibold : pal::Fonts().regular;
            dl->AddText(font, 15.0f, ImVec2(x + 50.0f, cy - 9.0f), ui::u32(textColor), kSteps[i].label);

            y += rowH;
        }
    }

    // Pie de la barra lateral
    {
        const float top = origin.y + height - footerBlock + 6.0f;
        dl->AddRectFilled(ImVec2(x, top), ImVec2(x + rowW, top + 54.0f), IM_COL32(255, 255, 255, 10), 10.0f);
        ui::drawIcon(dl, ui::Icon::Shield, ImVec2(x + 26.0f, top + 27.0f), 24.0f, ui::u32(pal::TextMuted), 1.6f);
        dl->AddText(pal::Fonts().medium, 13.0f, ImVec2(x + 50.0f, top + 10.0f), ui::u32(pal::Text), "Instalador v1.0");
        dl->AddText(pal::Fonts().regular, 12.0f, ImVec2(x + 50.0f, top + 28.0f), ui::u32(pal::TextMuted), "Modo demostración");
    }
}

void InstallerWindow::drawHeader(float left, float width)
{
    ImDrawList *dl = ImGui::GetWindowDrawList();
    const ImVec2 win = ImGui::GetWindowPos();
    const StepInfo &info = kSteps[indexOf(step_)];

    const float top = win.y + 30.0f;
    dl->AddText(pal::Fonts().semibold, 28.0f, ImVec2(win.x + left, top), ui::u32(pal::Text), info.title);
    dl->AddText(pal::Fonts().regular, 14.5f, ImVec2(win.x + left, top + 40.0f), ui::u32(pal::TextMuted), info.subtitle);

    // Indicador "Paso N de M"
    char counter[32];
    std::snprintf(counter, sizeof(counter), "Paso %d de %d", indexOf(step_) + 1, kStepCount);
    const ImVec2 extent = ui::measure(pal::Fonts().medium, 13.0f, counter);
    const float pillW = extent.x + 24.0f;
    const ImVec2 p0(win.x + left + width - pillW, top + 4.0f);
    const ImVec2 p1(p0.x + pillW, p0.y + 28.0f);
    dl->AddRectFilled(p0, p1, ui::u32(pal::withAlpha(pal::Text, 0.06f)), 14.0f);
    dl->AddRect(p0, p1, ui::u32(pal::withAlpha(pal::Text, 0.14f)), 14.0f);
    dl->AddText(pal::Fonts().medium, 13.0f, ImVec2(p0.x + 12.0f, p0.y + 7.0f), ui::u32(pal::TextMuted), counter);

    const float lineY = win.y + kHeaderHeight - 6.0f;
    dl->AddLine(ImVec2(win.x + left, lineY), ImVec2(win.x + left + width, lineY), ui::u32(pal::Separator), 1.0f);
}

// ---------------------------------------------------------------------------
void InstallerWindow::renderBody()
{
    switch (step_)
    {
    case InstallerStep::Welcome:
        renderWelcome();
        break;
    case InstallerStep::Terms:
        renderLegal(true);
        break;
    case InstallerStep::Privacy:
        renderLegal(false);
        break;
    case InstallerStep::Company:
        company_.render();
        break;
    case InstallerStep::Admin:
        admin_.render();
        break;
    case InstallerStep::Database:
        database_.render();
        break;
    case InstallerStep::Progress:
        progress_.render(started_);
        break;
    case InstallerStep::Finish:
    {
        const InstallSummary summary{company_.tradeName(), company_.city(), admin_.fullName(), admin_.username(),
                                     database_.host(), database_.port(), database_.database()};
        finish_.render(launch_, summary);
        break;
    }
    }
}

void InstallerWindow::renderWelcome()
{
    const float width = ImGui::GetContentRegionAvail().x;
    ImDrawList *dl = ImGui::GetWindowDrawList();

    // Tarjeta de bienvenida
    ui::cardBegin("##welcome_hero", ImVec2(0.0f, 0.0f), 28.0f, 26.0f);
    {
        const ImVec2 p = ImGui::GetCursorScreenPos();
        const float inner = ImGui::GetContentRegionAvail().x;
        ImDrawList *cdl = ImGui::GetWindowDrawList();

        ui::drawLogoMark(cdl, ImVec2(p.x + 34.0f, p.y + 34.0f), 30.0f, ui::u32(pal::Accent), ui::u32(pal::Text));
        cdl->AddText(pal::Fonts().semibold, 26.0f, ImVec2(p.x + 90.0f, p.y + 2.0f), ui::u32(pal::Text), "Bienvenido a Miscelánea Bellavista POS");

        const char *intro = "Vamos a preparar el sistema para tu negocio. El asistente te guiará por la configuración inicial "
                            "de la empresa, el usuario administrador y la conexión con la base de datos.";
        const float wrap = inner - 90.0f;
        cdl->AddText(pal::Fonts().regular, 15.0f, ImVec2(p.x + 90.0f, p.y + 40.0f), ui::u32(pal::TextMuted), intro, nullptr, wrap);
        const ImVec2 introSize = ui::measure(pal::Fonts().regular, 15.0f, intro, wrap);
        ImGui::Dummy(ImVec2(inner, std::max(76.0f, 40.0f + introSize.y)));
    }
    ui::cardEnd();

    ImGui::Dummy(ImVec2(0.0f, 4.0f));
    ui::sectionLabel("QUÉ VAMOS A CONFIGURAR");

    struct Tile
    {
        const char *number;
        const char *title;
        const char *text;
        ui::Icon icon;
    };
    const Tile tiles[4] = {
        {"01", "Datos de la empresa", "Identificación y datos generales del negocio.", ui::Icon::Building},
        {"02", "Cuenta administradora", "Creación del usuario principal del sistema.", ui::Icon::User},
        {"03", "Base de datos", "Configuración de la conexión con MySQL.", ui::Icon::Database},
        {"04", "Instalación", "Revisión y ejecución del proceso de configuración.", ui::Icon::Install},
    };

    const float gap = 14.0f;
    const bool single = width < 620.0f;
    const float tileW = single ? width : (width - gap) * 0.5f;
    const float tileH = 96.0f;

    for (int i = 0; i < 4; ++i)
    {
        if (!single && (i % 2) == 1)
            ImGui::SameLine(0.0f, gap);

        const ImVec2 p = ImGui::GetCursorScreenPos();
        const ImVec2 p1(p.x + tileW, p.y + tileH);
        dl->AddRectFilled(p, p1, ui::u32(pal::Card), 12.0f);
        dl->AddRect(p, p1, ui::u32(pal::Border), 12.0f, 0, 1.0f);

        const ImVec2 box0(p.x + 18.0f, p.y + 20.0f);
        const ImVec2 box1(box0.x + 54.0f, box0.y + 54.0f);
        dl->AddRectFilled(box0, box1, ui::u32(pal::withAlpha(pal::Accent, 0.20f)), 12.0f);
        ui::drawIcon(dl, tiles[i].icon, ImVec2((box0.x + box1.x) * 0.5f, (box0.y + box1.y) * 0.5f), 30.0f,
                     ui::u32(pal::AccentHover), 1.8f);

        dl->AddText(pal::Fonts().semibold, 15.5f, ImVec2(p.x + 88.0f, p.y + 20.0f), ui::u32(pal::Text), tiles[i].title);
        dl->AddText(pal::Fonts().regular, 13.5f, ImVec2(p.x + 88.0f, p.y + 44.0f), ui::u32(pal::TextMuted), tiles[i].text, nullptr, tileW - 88.0f - 46.0f);

        const ImVec2 ne = ui::measure(pal::Fonts().semibold, 12.5f, tiles[i].number);
        dl->AddText(pal::Fonts().semibold, 12.5f, ImVec2(p1.x - 18.0f - ne.x, p.y + 20.0f), ui::u32(pal::TextDisabled), tiles[i].number);

        ImGui::Dummy(ImVec2(tileW, tileH));
    }

    ImGui::Dummy(ImVec2(0.0f, 2.0f));
    ui::paragraph(pal::Fonts().regular, 13.5f, pal::TextMuted,
                  "Antes de continuar, revisarás los términos de uso y la política de privacidad.", width);
}

void InstallerWindow::renderLegal(bool terms)
{
    using namespace installer::legal;
    const LegalDocument document = terms ? termsDocument() : privacyDocument();
    const float width = ImGui::GetContentRegionAvail().x;

    ui::callout(ui::Kind::Warning, "Documento base — pendiente de revisión",
                "El contenido legal mostrado es una plantilla. Debe completarse con los datos reales del responsable "
                "y revisarse antes de distribuir el instalador.");

    const float checkBarHeight = 66.0f;
    const float spacing = ImGui::GetStyle().ItemSpacing.y;
    const float docHeight = std::max(140.0f, ImGui::GetContentRegionAvail().y - checkBarHeight - spacing);

    documentBegin(terms ? "##terms_doc" : "##privacy_doc", ImVec2(0.0f, docHeight));
    {
        const float inner = ImGui::GetContentRegionAvail().x;
        ui::label(pal::Fonts().semibold, 15.0f, pal::Text, document.title);
        ui::label(pal::Fonts().regular, 13.0f, pal::TextMuted, "Versión: [VERSIÓN]      Fecha de vigencia: [FECHA]");
        ImGui::Dummy(ImVec2(0.0f, 4.0f));
        {
            const ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddLine(p, ImVec2(p.x + inner, p.y), ui::u32(pal::Separator), 1.0f);
            ImGui::Dummy(ImVec2(0.0f, 8.0f));
        }

        for (std::size_t i = 0; i < document.count; ++i)
        {
            ImGui::Dummy(ImVec2(0.0f, 8.0f));
            ui::label(pal::Fonts().semibold, 14.5f, pal::PrimaryHover, document.sections[i].heading);
            ImGui::Dummy(ImVec2(0.0f, 4.0f));
            ui::paragraph(pal::Fonts().regular, 14.0f, pal::Text, document.sections[i].body, inner);
        }
    }
    documentEnd();

    ui::cardBegin(terms ? "##terms_accept" : "##privacy_accept", ImVec2(0.0f, checkBarHeight), 22.0f, 0.0f);
    {
        ImGui::Dummy(ImVec2(0.0f, 21.0f));
        if (terms)
            ui::checkbox("##accept_terms", "He leído y acepto los Términos y Condiciones.", &termsAccepted_);
        else
            ui::checkbox("##accept_privacy", "He leído la Política de Privacidad y conozco el tratamiento descrito.", &privacyAccepted_);
    }
    ui::cardEnd();
    (void)width;
}

// ---------------------------------------------------------------------------
bool InstallerWindow::canAdvance() const
{
    switch (step_)
    {
    case InstallerStep::Terms:
        return termsAccepted_;
    case InstallerStep::Privacy:
        return privacyAccepted_;
    case InstallerStep::Company:
        return company_.valid();
    case InstallerStep::Admin:
        return admin_.valid();
    case InstallerStep::Database:
        return database_.valid();
    default:
        return true;
    }
}

const char *InstallerWindow::footerHint() const
{
    switch (step_)
    {
    case InstallerStep::Welcome:
        return "Te guiaremos paso a paso.";
    case InstallerStep::Terms:
        return termsAccepted_ ? "Puedes continuar." : "Acepta los términos para continuar.";
    case InstallerStep::Privacy:
        return privacyAccepted_ ? "Puedes continuar." : "Confirma la lectura para continuar.";
    case InstallerStep::Company:
        return company_.valid() ? "Los campos con * son obligatorios." : (company_.tradeName()[0] ? "Revisa el correo electrónico." : "Completa los campos obligatorios (*).");
    case InstallerStep::Admin:
        return admin_.valid() ? "Los campos con * son obligatorios." : "Completa los datos y usa una contraseña de 8 o más caracteres.";
    case InstallerStep::Database:
        return database_.valid() ? "Todo listo para instalar." : "Completa los datos de conexión (*).";
    case InstallerStep::Progress:
        return "Por favor, no cierres esta ventana.";
    case InstallerStep::Finish:
        return "Gracias por instalar Miscelánea Bellavista POS.";
    }
    return "";
}

void InstallerWindow::drawFooter(float left, float width, float top, float height)
{
    ImDrawList *dl = ImGui::GetWindowDrawList();
    const ImVec2 win = ImGui::GetWindowPos();
    const float x0 = win.x + left;

    dl->AddLine(ImVec2(x0, win.y + top), ImVec2(x0 + width, win.y + top), ui::u32(pal::Separator), 1.0f);

    const float buttonH = 42.0f;
    const float y = top + (height - buttonH) * 0.5f;

    // Pista a la izquierda
    dl->AddText(pal::Fonts().regular, 13.5f, ImVec2(x0, win.y + top + height * 0.5f - 8.0f), ui::u32(pal::TextMuted), footerHint());

    const bool showBack = step_ == InstallerStep::Terms || step_ == InstallerStep::Privacy || step_ == InstallerStep::Company ||
                          step_ == InstallerStep::Admin || step_ == InstallerStep::Database;
    const bool showNext = step_ != InstallerStep::Progress;

    const float backW = 112.0f;
    const float nextW = step_ == InstallerStep::Terms ? 190.0f : 160.0f;
    const float gap = 12.0f;

    float total = showNext ? nextW : 0.0f;
    if (showBack)
        total += backW + (showNext ? gap : 0.0f);

    float x = left + width - total;
    if (showBack || showNext)
        ImGui::SetCursorPos(ImVec2(x, y));

    if (showBack)
    {
        if (ui::secondaryButton("Atrás", ImVec2(backW, buttonH)))
            back();
        x += backW + gap;
        ImGui::SetCursorPos(ImVec2(x, y));
    }

    if (showNext)
    {
        const char *label = "Continuar";
        switch (step_)
        {
        case InstallerStep::Welcome:
            label = "Comenzar";
            break;
        case InstallerStep::Terms:
            label = "Aceptar y seguir";
            break;
        case InstallerStep::Database:
            label = "Instalar";
            break;
        case InstallerStep::Finish:
            label = "Finalizar";
            break;
        default:
            break;
        }

        if (ui::primaryButton(label, ImVec2(nextW, buttonH), canAdvance()))
        {
            if (step_ == InstallerStep::Finish)
                wantsClose_ = true;
            else
                next();
        }
    }
}

// ---------------------------------------------------------------------------
void InstallerWindow::goToStep(InstallerStep step)
{
    step_ = step;
}

void InstallerWindow::next()
{
    switch (step_)
    {
    case InstallerStep::Welcome:
        step_ = InstallerStep::Terms;
        break;
    case InstallerStep::Terms:
        step_ = InstallerStep::Privacy;
        break;
    case InstallerStep::Privacy:
        step_ = InstallerStep::Company;
        break;
    case InstallerStep::Company:
        step_ = InstallerStep::Admin;
        break;
    case InstallerStep::Admin:
        step_ = InstallerStep::Database;
        break;
    case InstallerStep::Database:
        step_ = InstallerStep::Progress;
        started_ = true;
        progress_.start();
        break;
    default:
        break;
    }
}

void InstallerWindow::back()
{
    switch (step_)
    {
    case InstallerStep::Terms:
        step_ = InstallerStep::Welcome;
        break;
    case InstallerStep::Privacy:
        step_ = InstallerStep::Terms;
        break;
    case InstallerStep::Company:
        step_ = InstallerStep::Privacy;
        break;
    case InstallerStep::Admin:
        step_ = InstallerStep::Company;
        break;
    case InstallerStep::Database:
        step_ = InstallerStep::Admin;
        break;
    default:
        break;
    }
}
