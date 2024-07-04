#ifndef common_h
#define common_h 1

#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <random>

const std::size_t CHROMOSOME_SIZE = 2048, POPULATION_SIZE = 40'000;

std::random_device device;
std::default_random_engine engine(device());
std::bernoulli_distribution bernoulli;
auto rng_01 = std::bind(bernoulli, engine);
std::uniform_int_distribution<> uniform(0, CHROMOSOME_SIZE - 1);
auto rng_size = std::bind(uniform, engine);

template<typename chromosome> void initialize(chromosome &);
template<typename chromosome> void mutate(chromosome &);
template<typename chromosome> void crossover(chromosome &, chromosome &);
template<typename chromosome> std::size_t evaluate(chromosome &);

template<typename chromosome> int common_main(int argc, char *argv[])
{
    std::size_t step = std::numeric_limits<std::size_t>::max(), value = 0;

    if (argc > 1)
        step = std::atoi(argv[1]);

    if (step == 0)
        return value;

    std::vector<chromosome> population(POPULATION_SIZE);

    for (std::size_t i = 0; i < population.size(); ++i)
        initialize(population[i]);

    if (step == 1)
        return value;

    for (std::size_t i = 0; i < population.size(); i += 2)
    {
        mutate(population[i]);
        mutate(population[i + 1]);
        crossover(population[i], population[i + 1]);
        value += evaluate(population[i]) + evaluate(population[i + 1]);
    }

    return value;
}

#endif // common_h
