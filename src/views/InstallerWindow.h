#pragma once

#include "components/CompanySetupView.h"
#include "components/AdminSetupView.h"
#include "components/DatabaseSetupView.h"
#include "components/InstallProgressView.h"
#include "components/FinishView.h"

enum class InstallerStep
{
    Welcome,
    Terms,
    Privacy,
    Company,
    Admin,
    Database,
    Progress,
    Finish
};

class InstallerWindow
{
public:
    void render();

    // Verdadero cuando el usuario pulsa "Finalizar" en la última pantalla.
    bool wantsClose() const { return wantsClose_; }
    // Opción "Iniciar el sistema al finalizar" (para la versión funcional).
    bool launchAfterFinish() const { return launch_; }

private:
    void next();
    void back();
    void goToStep(InstallerStep step);

    void drawSidebar(float height, float sidebarWidth);
    void drawHeader(float left, float width);
    void drawFooter(float left, float width, float top, float height);

    void renderBody();
    void renderWelcome();
    void renderLegal(bool terms);

    bool canAdvance() const;
    const char *footerHint() const;

    InstallerStep step_ = InstallerStep::Welcome;

    CompanySetupView company_;
    AdminSetupView admin_;
    DatabaseSetupView database_;
    InstallProgressView progress_;
    FinishView finish_;

    bool termsAccepted_ = false;
    bool privacyAccepted_ = false;
    bool started_ = false;
    bool launch_ = true;
    bool wantsClose_ = false;
};
