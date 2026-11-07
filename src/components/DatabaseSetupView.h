#pragma once

class DatabaseSetupView
{
public:
    void render();
    bool valid() const;

    const char *host() const { return host_; }
    const char *port() const { return port_; }
    const char *database() const { return db_; }
    const char *user() const { return user_; }

private:
    char host_[128] = "127.0.0.1";
    char port_[8] = "3306";
    char db_[96] = "bellavista_pos";
    char user_[96] = "bellavista_app";
    char pass_[128] = "";

    bool local_ = true;
    bool revealPass_ = false;
    double testStartedAt_ = -1.0;
    bool testDone_ = false;
};
