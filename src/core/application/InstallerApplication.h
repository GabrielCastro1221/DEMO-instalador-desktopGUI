#pragma once

class InstallerApplication
{
public:
    InstallerApplication() = default;
    ~InstallerApplication() = default;

    InstallerApplication(const InstallerApplication &) = delete;
    InstallerApplication &operator=(const InstallerApplication &) = delete;

    int run();
};