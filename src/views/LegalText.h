#pragma once

#include <cstddef>

namespace installer::legal
{
    struct LegalSection
    {
        const char *heading;
        const char *body;
    };

    struct LegalDocument
    {
        const char *title;
        const LegalSection *sections;
        std::size_t count;
    };

    LegalDocument termsDocument();
    LegalDocument privacyDocument();
}
