#include "common.h"

#include <bitset>

///------------------------------------------------------------------------

class chromosome : public std::bitset<CHROMOSOME_SIZE>
{
public:
    chromosome() : std::bitset<CHROMOSOME_SIZE>() {}
};

//------------------------------------------------------------------------

constexpr char T(std::string_view s)
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

constexpr unsigned HIFF(std::string_view s)
{
    if (s.size() == 1)
        return 1;
    else
        return s.size() * f(T(s)) + HIFF(s.substr(0, s.size() / 2)) +
               HIFF(s.substr(s.size() / 2));
}

constexpr unsigned HIFF(const chromosome& c)
{
    return HIFF(c.to_string());
}

//------------------------------------------------------------------------

void initialize(chromosome &c)
{
    for (std::size_t bit = 0; bit < c.size(); ++bit)
        c[bit] = rng_ft();
}

//------------------------------------------------------------------------

void mutate(chromosome &c) { c.flip(rng_size()); }

//------------------------------------------------------------------------

void crossover(chromosome &c1, chromosome &c2)
{
    std::size_t t1 = rng_size(), t2 = rng_size();
    std::size_t p1 = std::min(t1, t2), p2 = std::max(t1, t2);

    for (std::size_t i = p1; i < p2; ++i)
    {
        bool tmp = c1[i];
        c1[i] = c2[i];
        c2[i] = tmp;
    }
}

//------------------------------------------------------------------------

std::size_t evaluate(const chromosome &c)
{
    // return c.count(); // onemax
    return HIFF(c);
}

//------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    return common_main<chromosome>(argc, argv);
}
