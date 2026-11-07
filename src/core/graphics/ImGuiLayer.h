#pragma once

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include <GLFW/glfw3.h>

class ImGuiLayer
{
public:
    ImGuiLayer() = default;
    ~ImGuiLayer();

    ImGuiLayer(const ImGuiLayer &) = delete;
    ImGuiLayer &operator=(const ImGuiLayer &) = delete;

    bool initialize(GLFWwindow *window);

    void beginFrame();
    void endFrame(GLFWwindow *window);

    void shutdown();

private:
    bool glewInitialized_ = false;
    bool imguiContextCreated_ = false;
    bool glfwBackendInitialized_ = false;
    bool openglBackendInitialized_ = false;
};