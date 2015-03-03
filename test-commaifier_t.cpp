#include ".test.hpp"
#include "commaifier_t.hpp"

TEST(basic)
{
    EQ("1", commaifier_t("1").str());
    EQ("12", commaifier_t("12").str());
    EQ("123", commaifier_t("123").str());
    EQ("1,234", commaifier_t("1234").str());
    EQ("12,345", commaifier_t("12345").str());
    EQ("123,456", commaifier_t("123456").str());
    EQ("1,234,567", commaifier_t("1234567").str());
    EQ("12,345,678", commaifier_t("12345678").str());
    EQ("123,456,789", commaifier_t("123456789").str());
    EQ("1,234,567,890", commaifier_t("1234567890").str());
}
