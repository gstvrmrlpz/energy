#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
#include <ranges>
#include <string>
#include <vector>

const std::size_t CHROMOSOME_SIZE = 1024, POPULATION_SIZE = 40'000;

class chromosome : public std::string
{
public:
    chromosome() : std::string(CHROMOSOME_SIZE, '0') {}
};

using population = std::vector<chromosome>;

std::random_device device;
std::default_random_engine engine(device());
std::bernoulli_distribution bernoulli(0.5);
auto rng_01 = std::bind(bernoulli, engine);
std::uniform_int_distribution<> uniform(0, CHROMOSOME_SIZE - 1);
auto rng_size = std::bind(uniform, engine);

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
        auto pos = rng_size();
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
    population p(POPULATION_SIZE);

    initialize(p);
    mutate(p);
    crossover(p);
    return evaluate(p);
}
