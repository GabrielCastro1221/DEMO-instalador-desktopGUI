#include "Fonts.h"

#include <filesystem>
#include <string>
#include <vector>

#include <cstdio>
#include <cstdlib>

#ifdef __linux__
#include <unistd.h>
#include <climits>
#endif

namespace fs = std::filesystem;

namespace installer::styles
{
    namespace
    {
        FontSet g_fonts;

        std::string executableDir()
        {
#ifdef __linux__
            char buffer[PATH_MAX];
            const ssize_t length = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
            if (length > 0)
            {
                buffer[length] = '\0';
                return fs::path(buffer).parent_path().string();
            }
#endif
            return ".";
        }

        // Carpetas donde se busca assets/fonts, de la más específica a la más general.
        std::vector<fs::path> assetFontDirectories()
        {
            std::vector<fs::path> dirs;
            const fs::path exe = executableDir();

            if (const char *custom = std::getenv("INSTALLER_ASSETS_DIR"))
                dirs.push_back(fs::path(custom) / "fonts");

            dirs.push_back(exe / "assets" / "fonts");
            dirs.push_back(exe / ".." / "assets" / "fonts");
            dirs.push_back(exe / ".." / ".." / "assets" / "fonts");
            dirs.push_back(fs::path("assets") / "fonts");
            dirs.push_back(fs::path("installer") / "assets" / "fonts");
#ifdef INSTALLER_ASSETS_DIR
            dirs.push_back(fs::path(INSTALLER_ASSETS_DIR) / "fonts");
#endif
            return dirs;
        }

        std::string firstExisting(const std::vector<fs::path> &dirs, const std::vector<std::string> &names)
        {
            for (const fs::path &dir : dirs)
            {
                for (const std::string &name : names)
                {
                    std::error_code ec;
                    const fs::path candidate = dir / name;
                    if (fs::is_regular_file(candidate, ec))
                        return candidate.string();
                }
            }
            return {};
        }

        ImFont *addFont(ImGuiIO &io, const std::string &path)
        {
            if (path.empty())
                return nullptr;
            return io.Fonts->AddFontFromFileTTF(path.c_str(), kBaseFontSize);
        }
    }

    FontSet &Fonts()
    {
        return g_fonts;
    }

    bool LoadFonts(ImGuiIO &io)
    {
        const std::vector<fs::path> assetDirs = assetFontDirectories();

        // 1) Inter incluida con el instalador (opción preferida).
        const std::string regularPath = firstExisting(assetDirs, {"Inter-Regular.otf", "Inter-Regular.ttf"});
        const std::string mediumPath = firstExisting(assetDirs, {"Inter-Medium.otf", "Inter-Medium.ttf"});
        const std::string semiboldPath = firstExisting(assetDirs, {"Inter-SemiBold.otf", "Inter-SemiBold.ttf"});

        g_fonts.regular = addFont(io, regularPath);
        if (g_fonts.regular)
        {
            g_fonts.medium = addFont(io, mediumPath);
            g_fonts.semibold = addFont(io, semiboldPath);
            if (!g_fonts.medium)
                g_fonts.medium = g_fonts.regular;
            if (!g_fonts.semibold)
                g_fonts.semibold = g_fonts.medium;
            io.FontDefault = g_fonts.regular;
            return true;
        }

        // 2) Fuentes del sistema (mismas que usa el Sistema POS).
        const std::vector<fs::path> systemDirs = {
            "/usr/share/fonts/truetype/ubuntu",
            "/usr/share/fonts/truetype/noto",
            "/usr/share/fonts/truetype/liberation",
            "/usr/share/fonts/truetype/dejavu",
        };
        const std::string systemRegular = firstExisting(systemDirs, {"Ubuntu-R.ttf", "NotoSans-Regular.ttf", "LiberationSans-Regular.ttf", "DejaVuSans.ttf"});
        const std::string systemBold = firstExisting(systemDirs, {"Ubuntu-M.ttf", "Ubuntu-B.ttf", "NotoSans-SemiBold.ttf", "NotoSans-Bold.ttf", "LiberationSans-Bold.ttf", "DejaVuSans-Bold.ttf"});

        g_fonts.regular = addFont(io, systemRegular);
        if (g_fonts.regular)
        {
            g_fonts.medium = g_fonts.regular;
            g_fonts.semibold = addFont(io, systemBold);
            if (!g_fonts.semibold)
                g_fonts.semibold = g_fonts.regular;
            io.FontDefault = g_fonts.regular;
            std::fprintf(stderr, "[AVISO] No se encontró la fuente Inter en assets/fonts; se usa una fuente del sistema.\n");
            return true;
        }

        // 3) Último recurso: fuente interna de Dear ImGui.
        std::fprintf(stderr, "[AVISO] No se encontró ninguna fuente TrueType; se usa la fuente interna de ImGui.\n");
        g_fonts.regular = io.Fonts->AddFontDefault();
        g_fonts.medium = g_fonts.regular;
        g_fonts.semibold = g_fonts.regular;
        io.FontDefault = g_fonts.regular;
        return false;
    }
}
