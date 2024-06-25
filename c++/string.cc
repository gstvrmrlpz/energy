#include "common.h"

#include <algorithm>
#include <string>
#include <vector>

class chromosome : public std::string
{
public:
    chromosome() : std::string(CHROMOSOME_SIZE, '0') {}
};

using population = std::vector<chromosome>;

void initialize(population &p)
{
    for (auto &i : p)
        for (std::size_t bit = 0; bit < i.size(); ++bit)
            i[bit] = rng_01() == false ? '0' : '1';
}

void mutate(population &p)
{
    for (auto &i : p)
    {
        std::size_t pos = rng_size();
        i[pos] = i[pos] == '0' ? '1' : '0';
    }
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
        count += std::ranges::count_if(i, [](char c) { return c == '1'; });
    return count;
}

int main(int argc, char *argv[])
{
    return common_main<population>(argc, argv);
}
