#include "kk.h"

#include "common.h"

#include <algorithm>
#include <vector>

void initialize(population &p)
{
    for (auto &i : p)
        for (std::size_t bit = 0; bit < i.size(); ++bit)
            i[bit] = rng_01();
}

void mutate(population &p)
{
    for (auto &i : p)
        i[rng_size()].flip();
}

void crossover(population &p)
{
    for (std::size_t i = 0; i < p.size(); i += 2)
        std::swap_ranges(
            p[i].begin(), p[i].begin() + rng_size(), p[i + 1].begin());
}

std::size_t evaluate(population &p)
{
    std::size_t count = 0;
    for (auto &i : p)
        count += std::count(i.cbegin(), i.cend(), true);
    return count;
}

int main() { return common_main(); }
