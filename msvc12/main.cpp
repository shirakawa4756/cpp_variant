#include <cmath>
#include <iostream>
#include <limits>
#include <type_traits>


#include <variant.hpp>

int main()
{
    variant::Variant v(std::numeric_limits<int>::max());
    std::cout << std::boolalpha << v.to_string() << std::endl;

    std::cout << std::to_string(true) << std::endl;

    return 0;
}