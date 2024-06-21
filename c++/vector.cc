#include <algorithm>
#include <bitset>
#include <functional>
#include <iostream>
#include <random>
#include <ranges>
#include <vector>

const std::size_t CHROMOSOME_SIZE = 2048, POPULATION_SIZE = 40'000;

class chromosome : public std::vector<bool>
{
public:
    chromosome() : std::vector<bool>(CHROMOSOME_SIZE) {}

    std::size_t count() const { return fitness; }

    void sync() { fitness = std::count(cbegin(), cend(), true); }

    friend std::ostream &operator<<(std::ostream &os, const chromosome &c)
    {
        for (const auto &i : c)
            os << i;
        return os << " - " << c.count();
    }

private:
    std::size_t fitness;
};

std::size_t onemax(const chromosome &c) { return c.count(); }

int main(int argc, char *argv[])
{
    std::random_device device;
    std::default_random_engine engine(device());

    std::vector<chromosome> population(POPULATION_SIZE);

    // init
    auto rng01 = std::bind(std::bernoulli_distribution(0.5), engine);
    for (auto &i : population)
        for (std::size_t bit = 0; bit < i.size(); ++bit)
            i[bit] = rng01();

    // mutation
    auto rng_chrom = std::bind(
        std::uniform_int_distribution<>(0, CHROMOSOME_SIZE - 1), engine);

    for (auto &i : population)
        i[rng_chrom()].flip();

    // crossover
    for (std::size_t i = 0; i < population.size(); i += 2)
        std::swap_ranges(population[i].begin(),
                         population[i].begin() + rng_chrom(),
                         population[i + 1].begin());

    // sync
    for (auto &i : population)
        i.sync();

    // sort
    std::ranges::sort(population,
                      [](const chromosome &a, const chromosome &b)
                      { return a.count() < b.count(); });

    return onemax(population[0]);
}
