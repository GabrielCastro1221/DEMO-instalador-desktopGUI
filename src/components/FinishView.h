#pragma once

struct InstallSummary
{
    const char *company;
    const char *city;
    const char *adminName;
    const char *adminUser;
    const char *dbHost;
    const char *dbPort;
    const char *dbName;
};

class FinishView
{
public:
    void render(bool &launch, const InstallSummary &summary);
};
