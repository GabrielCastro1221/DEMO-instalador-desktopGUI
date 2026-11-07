cmake_minimum_required(VERSION 3.16)

project(installer LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

get_filename_component(POS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

set(GLFW_DIR "${POS_ROOT}/external/glfw-3.5.1")
set(GLEW_DIR "${POS_ROOT}/external/glew-2.3.1")
set(IMGUI_DIR "${POS_ROOT}/external/imgui")

set(REQUIRED_FILES
    "${GLFW_DIR}/CMakeLists.txt"
    "${GLEW_DIR}/include/GL/glew.h"
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

foreach(FILE_PATH IN LISTS REQUIRED_FILES)
    if(NOT EXISTS "${FILE_PATH}")
        message(FATAL_ERROR "No se encuentra el archivo requerido:\n${FILE_PATH}")
    endif()
endforeach()

set(INSTALLER_SOURCES
    src/main.cpp
    src/core/application/InstallerApplication.cpp
    src/core/platform/GlfwWindow.cpp
    src/core/graphics/ImGuiLayer.cpp
    src/core/styles/Theme.cpp
    src/core/styles/Fonts.cpp
    src/components/UiKit.cpp
    src/views/InstallerWindow.cpp
    src/views/LegalText.cpp
    src/components/CompanySetupView.cpp
    src/components/AdminSetupView.cpp
    src/components/DatabaseSetupView.cpp
    src/components/InstallProgressView.cpp
    src/components/FinishView.cpp
    src/services/InstallationService.cpp
    src/services/DatabaseSetupService.cpp
    src/services/AdminSetupService.cpp
)

set(VALIDATED_SOURCES)

foreach(SOURCE_FILE IN LISTS INSTALLER_SOURCES)

    set(FULL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}")

    if(NOT EXISTS "${FULL_PATH}")
        message(FATAL_ERROR
            "\nERROR: NO SE ENCUENTRA UN ARCHIVO FUENTE\n"
            "Archivo: ${SOURCE_FILE}\n"
            "Ruta esperada: ${FULL_PATH}\n\n"
            "Verifica el nombre y la ubicación del archivo."
        )
    endif()
    list(APPEND VALIDATED_SOURCES "${FULL_PATH}")
endforeach()

find_library(GLEW_LIBRARY NAMES GLEW libGLEW.a PATHS "${GLEW_DIR}/lib" NO_DEFAULT_PATH)

if(NOT GLEW_LIBRARY)
    message(FATAL_ERROR "No se encontró GLEW en:\n${GLEW_DIR}/lib")
endif()

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory("${GLFW_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/glfw-build")

list(APPEND VALIDATED_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

add_executable(installer ${VALIDATED_SOURCES})

target_include_directories(installer PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/views"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/components"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/services"
    "${GLEW_DIR}/include"
    "${IMGUI_DIR}"
    "${IMGUI_DIR}/backends"
)

# Carpeta de recursos (fuentes). En desarrollo se usa la de las fuentes y, al compilar,
# se copia junto al ejecutable para poder distribuirlo.
target_compile_definitions(installer PRIVATE INSTALLER_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")

add_custom_command(TARGET installer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:installer>/assets"
    COMMENT "Copiando recursos del instalador (fuentes)"
)

target_link_libraries(installer PRIVATE glfw "${GLEW_LIBRARY}" OpenGL::GL ${CMAKE_DL_LIBS})

if(UNIX AND NOT APPLE)
    target_link_libraries(installer PRIVATE pthread)
endif()

set_target_properties(installer PROPERTIES OUTPUT_NAME "installer" BUILD_RPATH "${GLEW_DIR}/lib" INSTALL_RPATH "${GLEW_DIR}/lib")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(installer PRIVATE -O2 -DNDEBUG)
endif()

message(STATUS "")
message(STATUS "========================================")
message(STATUS "       NEXUM POS - INSTALLER")
message(STATUS "========================================")
message(STATUS "POS ROOT: ${POS_ROOT}")
message(STATUS "INSTALLER: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "GLFW: ${GLFW_DIR}")
message(STATUS "GLEW: ${GLEW_LIBRARY}")
message(STATUS "IMGUI: ${IMGUI_DIR}")
message(STATUS "")
message(STATUS "SOURCE FILES:")

foreach(SOURCE_FILE IN LISTS INSTALLER_SOURCES)
    message(STATUS "  [OK] ${SOURCE_FILE}")
endforeach()

message(STATUS "========================================")
message(STATUS "")
