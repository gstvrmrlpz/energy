#include "common.h"
#include "hiff.h"

#include <tr2/dynamic_bitset>

//------------------------------------------------------------------------

class chromosome : public std::tr2::dynamic_bitset<>
{
};

//------------------------------------------------------------------------

void initialize(chromosome &c, std::size_t size)
{
    c.resize(size);
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

std::size_t onemax(const chromosome &c) { return c.count(); }

//------------------------------------------------------------------------

std::size_t hiff(const chromosome &c) { return HIFF(c.to_string()); }

//------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    return common_main<chromosome>(argc, argv);
}
