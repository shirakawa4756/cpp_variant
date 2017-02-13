#include <cmath>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <limits>
#include <type_traits>
#include <variant/variant.hpp>

int main()
{
    setlocale(LC_ALL, "");
    _wsetlocale(LC_ALL, L"");

    variant::Variant values[] = {
            false, true, 0, 1, -10, 10, 0.0, 1.0, -10.1, 10.1,
            INT64_MAX, INT64_MIN,
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::min(),
            "false", "true", "0", "1", "-10", "10", "0.0", "1.0",
            "-10.2", "10.2", "100$", "-100$", "100.5$", "-100.5$",
            "9223372036854775807", "-9223372036854775806",
            "9223372036854775808", "-9223372036854775807"
            "1.79769e+308", " 2.22507e-308",
            "1.79769e+309", " 2.22507e-309",
            "anything", "Convert to?",
            "ì˙ñ{åÍëÂè‰ïv?" };

    std::cout << "Å† Convert to Double! (tryCast)" << std::endl;
    for (const auto &v : values) {
        double result = 0;
        if (v.tryCast(result)) {
            std::cout << "Converted: "  
                      << std::noboolalpha
                      << std::setw(5) << v <<" --> "
                      << std::setw(6) << std::boolalpha << result
                      << std::endl;
        } else {
            std::cout << "Attempted: " << v << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "Å† Convert to wchar_t." << std::endl;
    for (const auto &v : values) {
        std::wcout << v << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Å† Convert to various types using cast." << std::endl;
    for (const auto &v : values) {
        bool         b   = v;
        int32_t      i32 = v;
        int64_t      i64 = v;
        double       d   = v;
        std::string  s   = v;
        std::wstring ws  = v;
        std::cout << v   << ", "
                  << b   << ", "
                  << i32 << ", "
                  << i64 << ", "
                  << d   << ", "
                  << s   << std::endl;
    }
    std::cout << std::endl;

    return 0;
}