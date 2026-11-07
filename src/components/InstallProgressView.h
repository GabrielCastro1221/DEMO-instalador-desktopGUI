#pragma once

class InstallProgressView
{
public:
    void start();
    void render(bool started);
    bool finished() const { return finished_; }

private:
    float progress_ = 0;
    double last_ = 0;
    bool finished_ = false;
};
