#include <iostream>
#include "num2words.hpp"

int main(int, char *[])
{

    std::cout << 
    eg::numbers::num2words(18'446'744'073'709'551'615ull) << '\n';


    return EXIT_SUCCESS;
}
