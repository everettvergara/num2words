#include <string_view>
#include <array>
#include <cassert> 
#include <concepts>
#include <limits>


    template<typename T>
    struct make_unsigned_ext
    {
        using type = std::make_unsigned_t<T>;
    };

    template<>
    struct make_unsigned_ext<__int128>
    {
        using type = unsigned __int128;
    };

    template<>
    struct make_unsigned_ext<unsigned __int128>
    {
        using type = unsigned __int128;
    };

    template<typename T>
    using make_unsigned_ext_t = typename make_unsigned_ext<T>::type;



namespace eg::numbers
{
    constexpr std::array<std::string_view, 10> k_num_100_900  
    {  
       "","one hundred","two hundred","three hundred","four hundred","five hundred","six hundred","seven hundred","eight hundred","nine hundred" 
    };

    // max: unsigned 128-bit (2^128 -1);
    // 340,282,366,920,938,463,463,374,607,431,768,211,455
    constexpr std::array<std::string_view, 15> k_muls  
    { 
       "","thousand","million","billion","trillion","quadrillion","quintillion",
       "sixtillion","septillion","octillion","nonillion","decillion","undecillion","duodecillion","tredecillion"
    };

    constexpr std::array<std::string_view, 100> k_num_00_99 
    {
       "","one","two","three","four","five","six","seven","eight","nine",
       "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen",
       "twenty","twenty one","twenty two","twenty three","twenty four","twenty five","twenty six","twenty seven","twenty eight","twenty nine",
       "thirty","thirty one","thirty two","thirty three","thirty four","thirty five","thirty six","thirty seven","thirty eight","thirty nine",
       "forty","forty one","forty two","forty three","forty four","forty five","forty six","forty seven","forty eight","forty nine",
       "fifty","fifty one","fifty two","fifty three","fifty four","fifty five","fifty six","fifty seven","fifty eight","fifty nine",
       "sixty","sixty one","sixty two","sixty three","sixty four","sixty five","sixty six","sixty seven","sixty eight","sixty nine",
       "seventy","seventy one","seventy two","seventy three","seventy four","seventy five","seventy six","seventy seven","seventy eight","seventy nine",
       "eighty","eighty one","eighty two","eighty three","eighty four","eighty five","eighty six","eighty seven","eighty eight","eighty nine",
       "ninety","ninety one","ninety two","ninety three","ninety four","ninety five","ninety six","ninety seven","ninety eight","ninety nine"
    };

    constexpr size_t k_stack_sz {k_muls.size() * 2};
    constexpr std::array<std::string_view, 2> k_sign {"","negative"};
    constexpr std::string_view k_zero {"zero"};


    template<typename T>
    requires ((std::integral<T> or std::same_as<T, __int128> or std::same_as<T, unsigned __int128>) and not std::same_as<T, bool>)
    std::string num2words(T n)
    {
        static_assert(std::numeric_limits<make_unsigned_ext_t<T>>::digits10 <= 42, "num2words can support up to tredecillion only!");

        using unsigned_t = make_unsigned_ext_t<T>;

        unsigned_t u {0};
        size_t s_ix {0};

        if constexpr (std::signed_integral<T>)
        {
            // Safe conversion for min limits
            u = n < 0 ? static_cast<unsigned_t>(-(n + 1)) + 1 : static_cast<unsigned_t>(n);
            s_ix = n < 0;
        }

        else 
        {
            u = n;
            s_ix = 0;
        }

        if (u == 0)
        {
            return std::string {k_zero};
        }

        if (u < 100) 
        {
            return std::string{k_num_00_99[u]};
        }

        std::array<int, k_stack_sz> ixs {0};
        size_t ixs_i {0};
        size_t str_sz {0};
        size_t segment_count {0};

        for (size_t m {0};; ++m)
        {
            const size_t num_2digit_ix {static_cast<size_t>(u % 100)};
            const size_t num_3digit_ix {static_cast<size_t>((u / 100) % 10)};

            u /= 1'000;

            ixs[ixs_i++] = num_2digit_ix;   // push
            ixs[ixs_i++] = num_3digit_ix;   // push

            if (num_3digit_ix > 0)
            {
                str_sz += k_num_100_900[num_3digit_ix].size();
                ++segment_count;
            }

            if (num_2digit_ix > 0)
            {
                str_sz += k_num_00_99[num_2digit_ix].size();
                ++segment_count;
            }

            if (num_3digit_ix > 0 || num_2digit_ix > 0)
            {
                str_sz += k_muls[m].size();
                ++segment_count;
            }

            if (u == 0)
            {
                break;
            }
        }

        if (s_ix > 0)
        {
            str_sz += k_sign[s_ix].size();
            ++segment_count;
        }

        const size_t space_count = segment_count > 0 ? segment_count - 1 : 0;
        
        size_t muls_ix {ixs_i / 2 - 1};
        
        std::string num_words;
        num_words.reserve(str_sz + space_count);

        if (s_ix > 0)
        {
            num_words.append(k_sign[s_ix]);
        }

        const auto append_with_space =
                [&](const std::string_view word)
                {
                    if (not num_words.empty())
                    {
                        num_words.push_back(' ');
                    }

                    num_words.append(word);
                };

        do
        {
            const auto num_3digit_ix = ixs[--ixs_i];    // pop
            const auto num_2digit_ix = ixs[--ixs_i];    // pop

            if (num_3digit_ix > 0)
            {
                append_with_space(k_num_100_900[num_3digit_ix]);
            }

            if (num_2digit_ix > 0)
            {
                append_with_space(k_num_00_99[num_2digit_ix]);
            }

            if (num_3digit_ix > 0 or num_2digit_ix > 0)
            {
                append_with_space(k_muls[muls_ix]);
            }

            --muls_ix;
        } while (ixs_i > 0);

        return num_words;
    }
}