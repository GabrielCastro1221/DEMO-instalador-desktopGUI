#pragma once

class AdminSetupView
{
public:
    void render();
    bool valid() const;

    const char *fullName() const { return name_; }
    const char *username() const { return user_; }

private:
    char name_[128] = "";
    char user_[64] = "admin";
    char pass_[128] = "";
    char confirm_[128] = "";
    bool revealPass_ = false;
    bool revealConfirm_ = false;
};
