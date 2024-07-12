#include "common.h"
#include "hiff.h"

#include <algorithm>
#include <vector>

//------------------------------------------------------------------------

class chromosome : public std::vector<bool>
{
public:
    chromosome() : std::vector<bool>(CHROMOSOME_SIZE) {}
};

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

std::size_t evaluate(const chromosome &c)
{
    // return std::ranges::count(c, true); } // onemax
    return HIFF(c.begin(), c.end()); // HIFF
}

//------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    return common_main<chromosome>(argc, argv);
}
