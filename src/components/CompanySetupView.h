#pragma once

class CompanySetupView
{
public:
    void render();
    bool valid() const;

    const char *tradeName() const { return trade_; }
    const char *city() const { return city_; }
    const char *documentType() const { return type_; }
    const char *documentNumber() const { return number_; }

private:
    bool emailLooksValid() const;

    char trade_[128] = "Miscelánea Bellavista";
    char legal_[128] = "";
    char type_[24] = "NIT";
    char number_[48] = "";
    char phone_[48] = "";
    char email_[128] = "";
    char city_[80] = "Manizales";
    char address_[160] = "";
    char currency_[48] = "COP - Peso colombiano";
};
