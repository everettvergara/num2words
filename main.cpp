#include <iostream>
#include "num2words.hpp"

int main(int, char *[])
{
    const auto max_signed   = std::numeric_limits<__int128>::max();
    const auto max_unsigned = std::numeric_limits<unsigned __int128>::max();

    std::cout << 18'446'744'073'709'551'615ull << "\n" <<
    eg::numbers::num2words(18'446'744'073'709'551'615ull) << "\n\n"; 
   

    // 170'141'183'460'469'231'731'687'303'715'884'105'727
    std::cout <<  
    eg::numbers::num2words(max_signed) << "\n\n"; 

    // 340'282'366'920'938'463'463'374'607'431'768'211'455
    std::cout << 
    eg::numbers::num2words(max_unsigned)  << "\n\n"; 

    return EXIT_SUCCESS;
}
