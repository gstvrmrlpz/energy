#include "common.h"

#include <algorithm>
#include <vector>

//------------------------------------------------------------------------

class chromosome : public std::vector<bool>
{
public:
    chromosome() : std::vector<bool>(CHROMOSOME_SIZE) {}
};

//------------------------------------------------------------------------

template<class iterator> constexpr char T(iterator begin, iterator end)
{
    std::size_t size = end - begin;
    switch (size)
    {
        case 1:
            return '0' + *begin;
            break;
        case 2:
            return t('0' + *begin, '0' + *(begin + 1));
            break;
        default:
            return t(T(begin, begin + size / 2), T(begin + size / 2, end));
            break;
    }
}

//------------------------------------------------------------------------

template<class iterator>
constexpr unsigned HIFF(iterator begin, iterator end)
{
    std::size_t size = end - begin;
    if (size == 1)
        return 1;
    else
        return size * f(T(begin, end)) + HIFF(begin, begin + size / 2) +
               HIFF(begin + size / 2, end);
}

//------------------------------------------------------------------------

void initialize(chromosome &c)
{
    for (std::size_t bit = 0; bit < c.size(); ++bit)
        c[bit] = rng_ft();
}

//------------------------------------------------------------------------

void mutate(chromosome &c) { c[rng_size()].flip(); }

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
    // return std::ranges::count(c, true); } // onemax
    return HIFF(c.cbegin(), c.cend()); // HIFF
}

//------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    return common_main<chromosome>(argc, argv);
}
