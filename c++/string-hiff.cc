#include "common.h"

#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>

//------------------------------------------------------------------------

constexpr char t(const char left, const char right)
{
    if (left == right)
        return left;
    else
        return '-';
}

// t test
static_assert(t('0', '0') == '0');
static_assert(t('0', '1') == '-');
static_assert(t('1', '0') == '-');
static_assert(t('1', '1') == '1');
static_assert(t('0', '-') == '-');
static_assert(t('-', '1') == '-');

//------------------------------------------------------------------------

constexpr char T(const std::string_view &s)
{
    switch (s.size())
    {
        case 1:
            return s.front();
            break;
        case 2:
            return t(s.front(), s.back());
            break;
        default:
            return t(T(s.substr(0, s.size() / 2)),
                     T(s.substr(s.size() / 2, s.size())));
            break;
    }
}

// T test
static_assert(T("0") == '0');
static_assert(T("1") == '1');
static_assert(T("-") == '-');
static_assert(T("00") == '0');
static_assert(T("01") == '-');
static_assert(T("10") == '-');
static_assert(T("11") == '1');
static_assert(T("0-") == '-');
static_assert(T("-1") == '-');
static_assert(T("0000") == '0');
static_assert(T("0-00") == '-');
static_assert(T("0011") == '-');
static_assert(T("11-1") == '-');
static_assert(T("1111") == '1');

//------------------------------------------------------------------------

constexpr unsigned f(const char c)
{
    if (c == '-')
        return 0;
    else
        return 1;
}

// f test
static_assert(f('0') == 1);
static_assert(f('1') == 1);
static_assert(f('-') == 0);

//------------------------------------------------------------------------

constexpr unsigned HIFF(const std::string_view &s)
{
    if (s.size() == 1)
        return 1;
    else
        return s.size() * f(T(s)) + HIFF(s.substr(0, s.size() / 2)) +
               HIFF(s.substr(s.size() / 2));
}

// HIFF test

static_assert(HIFF("0") == 1);
static_assert(HIFF("1") == 1);
static_assert(HIFF("00") == 4);
static_assert(HIFF("01") == 2);
static_assert(HIFF("10") == 2);
static_assert(HIFF("11") == 4);
static_assert(HIFF("0011") == 8);
static_assert(HIFF("1111") == 12);
static_assert(HIFF("10110000") == 18);
static_assert(HIFF("00001101") == 18);

//------------------------------------------------------------------------

class chromosome : public std::string
{
public:
    chromosome() : std::string(CHROMOSOME_SIZE, '0') {}
};

//------------------------------------------------------------------------

void initialize(chromosome &c)
{
    for (std::size_t bit = 0; bit < c.size(); ++bit)
        c[bit] = rng_01();
}

//------------------------------------------------------------------------

void mutate(chromosome &c)
{
    std::size_t pos = rng_size();
    c[pos] = '0' + '1' - c[pos];

}

//------------------------------------------------------------------------

void crossover(chromosome &c1, chromosome &c2)
{
    std::size_t t1 = rng_size(), t2 = rng_size();
    std::size_t p1 = std::min(t1, t2), p2 = std::max(t1, t2);
    std::swap_ranges(c1.begin() + p1, c1.begin() + p2, c2.begin() + p1);
}

//------------------------------------------------------------------------

std::size_t evaluate(chromosome &c)
{
    // return std::ranges::count(c, '1'); // onemax
    return HIFF(c);
}

//------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    return common_main<chromosome>(argc, argv);
}

//------------------------------------------------------------------------
