#include "common.h"
#include "hiff.h"

#include <algorithm>
#include <vector>

//------------------------------------------------------------------------

class chromosome : public std::vector<char>
{
};

//------------------------------------------------------------------------

void initialize(chromosome &c, std::size_t size)
{
    c.resize(size);
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

std::size_t onemax(const chromosome &c)
{
#if __cplusplus > 201703L
    return std::ranges::count(c, true);
#else
    return std::count(c.begin(), c.end(), true);
#endif
}

//------------------------------------------------------------------------

std::size_t hiff(const chromosome &c) { return HIFF(c.begin(), c.end()); }

//------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    return common_main<chromosome>(argc, argv);
}
