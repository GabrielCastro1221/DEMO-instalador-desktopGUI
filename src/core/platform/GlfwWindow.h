#pragma once

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include <GLFW/glfw3.h>

class GlfwWindow
{
public:
    GlfwWindow() = default;
    ~GlfwWindow();

    GlfwWindow(const GlfwWindow &) = delete;
    GlfwWindow &operator=(const GlfwWindow &) = delete;

    bool initialize();
    void pollEvents();
    bool shouldClose() const;

    GLFWwindow *get() const;

    void shutdown();

private:
    static void glfwErrorCallback(int error, const char *description);

private:
    GLFWwindow *window_ = nullptr;
    bool initialized_ = false;
};
