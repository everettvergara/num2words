#include <string_view>
#include <array>
#include <cassert> 
#include <concepts>
#include <limits>

namespace eg::numbers
{
    static constexpr std::array<std::string_view, 10> k_num_100_900 = 
    {  
        "", "one hundred ", "two hundred ", "three hundred ", "four hundred ", "five hundred ", "six hundred ", "seven hundred ", "eight hundred ", "nine hundred" 
    };

    // max: ull 64-bit
    //
    // 18,446,744,073,709,551,615
    //
    // quintillion
    //    quadrillion
    //        trillion
    //            billion
    //                million
    //                    thousand
    //                        -

    static constexpr std::array<std::string_view, 7> k_muls = 
    { 
        "", "thousand ", "million ", "billion ", "trillion ", "quadrillion ", "quintillion " 
        //, "sixtillion ", "septillion ", "octillion ", "nonillion ", "decillion" 
    };

    static constexpr std::array<std::string_view, 100> k_num_00_99 =
    {
        "", "one ","two ","three ","four ","five ","six ","seven ","eight ","nine ",
        "ten ","eleven ","twelve ","thirteen ","fourteen ","fifteen ","sixteen ","seventeen ","eighteen ","nineteen ",
        "twenty ","twenty one ","twenty two ","twenty three ","twenty four ","twenty five ","twenty six ","twenty seven ","twenty eight ","twenty nine ",
        "thirty ","thirty one ","thirty two ","thirty three ","thirty four ","thirty five ","thirty six ","thirty seven ","thirty eight ","thirty nine ",
        "forty ","forty one ","forty two ","forty three ","forty four ","forty five ","forty six ","forty seven ","forty eight ","forty nine ",
        "fifty ","fifty one ","fifty two ","fifty three ","fifty four ","fifty five ","fifty six ","fifty seven ","fifty eight ","fifty nine ",
        "sixty ","sixty one ","sixty two ","sixty three ","sixty four ","sixty five ","sixty six ","sixty seven ","sixty eight ","sixty nine ",
        "seventy ","seventy one ","seventy two ","seventy three ","seventy four ","seventy five ","seventy six ","seventy seven ","seventy eight ","seventy nine ",
        "eighty ","eighty one ","eighty two ","eighty three ","eighty four ","eighty five ","eighty six ","eighty seven ","eighty eight ","eighty nine ",
        "ninety ","ninety one ","ninety two ","ninety three ","ninety four ","ninety five ","ninety six ","ninety seven ","ninety eight ","ninety nine"
    };

    static constexpr auto k_stack_sz = k_muls.size() * 2;

    template<std::integral T>
    static std::string num2words(T n)
    {
        static_assert(sizeof(void*) == 8, "Can be compiled only with 64-bit systems.");

        if constexpr (std::signed_integral<T>) 
        {
            if (n == INT_MIN)
            {
                return std::string{k_num_00_99.front()};
            }

            if (n < 0)
            {
                n *= -1;
            }
        }

        if (n < 100) 
        {
            return std::string{k_num_00_99[n]};
        }

        std::array<int, k_stack_sz> ixs {0};
        auto ixs_i {0uz};
        auto str_sz {0uz};

        using unsigned_t = std::make_unsigned_t<T>;

        for (auto m {0uz};;)
        {
            size_t num_2digit_ix {static_cast<unsigned_t>(n % 100)};
            size_t num_3digit_ix {static_cast<unsigned_t>((n / 100) % 10)};
            
            n /= 1'000;

            ixs[ixs_i++] = num_2digit_ix;   // push
            ixs[ixs_i++] = num_3digit_ix;   // push

            str_sz += k_num_00_99[num_2digit_ix].size();
            str_sz += k_num_100_900[num_3digit_ix].size();
            str_sz += (num_2digit_ix > 0 or num_3digit_ix > 0) ? k_muls[m].size() : 0;

            if (n == 0) 
            {
                break;
            }
            
            m += 1;
        } 

        auto muls_ix {ixs_i / 2uz - 1uz};

        std::string num_words;
        num_words.reserve(str_sz);

        do
        {
            const auto num_3digit_ix = ixs[--ixs_i];    // pop
            const auto num_2digit_ix = ixs[--ixs_i];    // pop

            num_words.append(k_num_100_900[num_3digit_ix]);
            num_words.append(k_num_00_99[num_2digit_ix]);

            if (num_3digit_ix > 0 or num_2digit_ix > 0)
            {
                num_words.append(k_muls[muls_ix]);
            }

            --muls_ix;
        } while (ixs_i > 0);

        assert(str_sz == num_words.capacity());
        
        return num_words;
    }
}