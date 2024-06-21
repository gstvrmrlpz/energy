#include <algorithm>
#include <array>
#include <boost/dynamic_bitset.hpp>
#include <functional>
#include <iostream>
#include <random>
#include <ranges>
#include <vector>

const std::size_t CHROMOSOME_SIZE = 2048, POPULATION_SIZE = 40'000;

class chromosome : public boost::dynamic_bitset<>
{
public:
    chromosome() : boost::dynamic_bitset<>(CHROMOSOME_SIZE) {}

    friend std::ostream &operator<<(std::ostream &os, const chromosome &c)
    {
        return os << c << " - " << c.count();
    }
};

std::size_t onemax(const chromosome &c) { return c.count(); }

int main(int argc, char *argv[])
{
    std::random_device device;
    std::default_random_engine engine(device());

    std::array<chromosome, POPULATION_SIZE> population;

    // init
    auto rng01 = std::bind(std::bernoulli_distribution(0.5), engine);
    for (auto &i : population)
        for (std::size_t bit = 0; bit < i.size(); ++bit)
            i[bit] = rng01();

    // mutation
    auto rng_chrom = std::bind(
        std::uniform_int_distribution<>(0, CHROMOSOME_SIZE - 1), engine);

    for (auto &i : population)
        i.flip(rng_chrom());

    // crossover
    for (std::size_t i = 0; i < population.size(); i += 2)
        for (std::size_t j = rng_chrom(); j < population[i].size(); ++j)
        {
            bool tmp = population[i][j];
            population[i][j] = population[i + 1][j];
            population[i + 1][j] = tmp;
        }

    // sort
    std::ranges::sort(population,
                      [](const chromosome &a, const chromosome &b)
                      { return a.count() < b.count(); });

    return onemax(population[0]);
}
