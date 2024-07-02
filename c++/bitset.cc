#include "common.h"

#include <tr2/dynamic_bitset>
#include <vector>

class chromosome : public std::tr2::dynamic_bitset<>
{
public:
    chromosome() : std::tr2::dynamic_bitset<>(CHROMOSOME_SIZE) {}
};

using population = std::vector<chromosome>;

void initialize(population &p)
{
    for (auto &i : p)
        for (std::size_t bit = 0; bit < i.size(); ++bit)
            i[bit] = rng_01();
}

void mutate(population &p)
{
    for (auto &i : p)
        i.flip(rng_size());
}

void crossover(population &p)
{
    for (std::size_t i = 0; i < p.size(); i += 2)
        for (std::size_t j = rng_size(); j < p[i].size(); ++j)
        {
            bool tmp = p[i][j];
            p[i][j] = p[i + 1][j];
            p[i + 1][j] = tmp;
        }
}

std::size_t evaluate(population &p)
{
    std::size_t count = 0;
    for (auto &i : p)
        count += i.count();
    return count;
}

int main(int argc, char *argv[])
{
    return common_main<population>(argc, argv);
}
