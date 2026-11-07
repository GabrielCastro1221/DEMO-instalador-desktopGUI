#include <GL/glew.h>

#include "ImGuiLayer.h"

#include <imgui.h>
#include "core/styles/Fonts.h"
#include "core/styles/Theme.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <cstdio>

ImGuiLayer::~ImGuiLayer()
{
    shutdown();
}

bool ImGuiLayer::initialize(GLFWwindow *window)
{
    if (!window)
    {
        std::fprintf(stderr, "[ERROR] Ventana GLFW no válida.\n");
        return false;
    }

    glewExperimental = GL_TRUE;
    const GLenum glewStatus = glewInit();

    if (glewStatus != GLEW_OK)
    {
        std::fprintf(stderr, "[ERROR] GLEW: %s\n", reinterpret_cast<const char *>(glewGetErrorString(glewStatus)));
        return false;
    }

    glewInitialized_ = true;
    glGetError();
    std::printf("[4/7] GLEW inicializado correctamente.\n");
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    imguiContextCreated_ = true;

    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr; // el instalador no guarda posiciones de ventanas

    ImGui::StyleColorsDark();
    installer::styles::Theme::Apply();
    installer::styles::LoadFonts(io);

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
    {
        std::fprintf(stderr, "[ERROR] No se pudo inicializar el backend GLFW de ImGui.\n");
        shutdown();
        return false;
    }

    glfwBackendInitialized_ = true;

    if (!ImGui_ImplOpenGL3_Init("#version 330"))
    {
        std::fprintf(stderr, "[ERROR] No se pudo inicializar el backend OpenGL de ImGui.\n");
        shutdown();
        return false;
    }

    openglBackendInitialized_ = true;
    return true;
}

void ImGuiLayer::beginFrame()
{
    if (!imguiContextCreated_ || !glfwBackendInitialized_ || !openglBackendInitialized_)
    {
        return;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
}

void ImGuiLayer::endFrame(GLFWwindow *window)
{
    if (!window || !imguiContextCreated_ || !glfwBackendInitialized_ || !openglBackendInitialized_)
    {
        return;
    }

    ImGui::Render();

    int framebufferWidth = 0;
    int framebufferHeight = 0;

    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glClearColor(0.11f, 0.12f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

void ImGuiLayer::shutdown()
{
    if (openglBackendInitialized_)
    {
        ImGui_ImplOpenGL3_Shutdown();
        openglBackendInitialized_ = false;
    }

    if (glfwBackendInitialized_)
    {
        ImGui_ImplGlfw_Shutdown();
        glfwBackendInitialized_ = false;
    }

    if (imguiContextCreated_)
    {
        ImGui::DestroyContext();
        imguiContextCreated_ = false;
    }

    glewInitialized_ = false;
}