#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>
#include <type_traits>
#include <variant.hpp>

int main()
{
    variant::Variant values[] = { "test string context",
            false, true, 0, 1, -10, 10, 0.0, 1.0, -10.1, 10.1,
            "false", "true", "0", "1", "-10", "10", "0.0", "1.0",
            "-10.2", "10.2", "anything", "Convert to?" };

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

    for (const auto &v : values) {
        std::wcout << v << std::endl;
    }

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


    return 0;
}