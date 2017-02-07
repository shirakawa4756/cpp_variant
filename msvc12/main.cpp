#include <cmath>
#include <iostream>
#include <limits>
#include <type_traits>
#include <variant.hpp>

int main()
{
    try {
        variant::Variant v = true;
        std::cout << std::boolalpha << v << std::endl;

        v = 100;
        std::cout << v << std::endl;

        v = 100.5;
        std::cout << v << std::endl;

        v = "anything";
        std::cout << v << std::endl;

        int i = v.toInt();
        std::cout << i << std::endl;
    } catch (const std::exception &e) {
        std::cout << "bat cast: " << e.what() << std::endl;
    }
    return 0;
}