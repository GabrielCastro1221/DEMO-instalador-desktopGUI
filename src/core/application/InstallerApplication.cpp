#include "InstallerApplication.h"
#include "core/platform/GlfwWindow.h"
#include "core/graphics/ImGuiLayer.h"
#include "views/InstallerWindow.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <cstdio>

int InstallerApplication::run()
{
    std::printf("[1/7] Iniciando instalador...\n");
    std::fflush(stdout);

    GlfwWindow window;

    if (!window.initialize())
    {
        return 1;
    }

    std::printf("[2/7] GLFW inicializado correctamente.\n");
    std::printf("[3/7] Ventana GLFW creada correctamente.\n");

    ImGuiLayer gui;

    if (!gui.initialize(window.get()))
    {
        return 1;
    }

    std::printf("[5/7] Dear ImGui inicializado correctamente.\n");

    InstallerWindow installer;

    std::printf("[6/7] InstallerWindow creado.\n");
    std::printf("[7/7] Iniciando interfaz gráfica...\n");

    while (!window.shouldClose())
    {
        window.pollEvents();
        gui.beginFrame();
        installer.render();
        gui.endFrame(window.get());

        if (installer.wantsClose())
            glfwSetWindowShouldClose(window.get(), GLFW_TRUE);
    }
    gui.shutdown();
    window.shutdown();
    std::printf("[OK] Instalador cerrado correctamente.\n");

    return 0;
}