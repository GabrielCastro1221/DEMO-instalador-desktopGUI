#include "GlfwWindow.h"
#include <cstdio>

GlfwWindow::~GlfwWindow()
{
    shutdown();
}

void GlfwWindow::glfwErrorCallback(int error, const char *description)
{
    std::fprintf(stderr, "[GLFW ERROR %d] %s\n", error, description);
}

bool GlfwWindow::initialize()
{
    glfwSetErrorCallback(glfwErrorCallback);

    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

    if (!glfwInit())
    {
        std::fprintf(stderr, "[ERROR] No se pudo iniciar GLFW.\n");
        return false;
    }

    initialized_ = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(1180, 760, "Miscelánea Bellavista POS - Instalador", nullptr, nullptr);

    if (!window_)
    {
        std::fprintf(stderr, "[ERROR] No se pudo crear la ventana GLFW.\n");
        shutdown();
        return false;
    }

    glfwSetWindowSizeLimits(window_, 980, 640, GLFW_DONT_CARE, GLFW_DONT_CARE);

    // Centrar la ventana en el monitor principal.
    if (GLFWmonitor *monitor = glfwGetPrimaryMonitor())
    {
        int mx = 0, my = 0, mw = 0, mh = 0;
        glfwGetMonitorWorkarea(monitor, &mx, &my, &mw, &mh);
        int ww = 0, wh = 0;
        glfwGetWindowSize(window_, &ww, &wh);
        if (mw > 0 && mh > 0)
            glfwSetWindowPos(window_, mx + (mw - ww) / 2, my + (mh - wh) / 2);
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);
    return true;
}

void GlfwWindow::pollEvents()
{
    glfwPollEvents();
}

bool GlfwWindow::shouldClose() const
{
    return !window_ || glfwWindowShouldClose(window_);
}

GLFWwindow *GlfwWindow::get() const
{
    return window_;
}

void GlfwWindow::shutdown()
{
    if (window_)
    {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }

    if (initialized_)
    {
        glfwTerminate();
        initialized_ = false;
    }
}
