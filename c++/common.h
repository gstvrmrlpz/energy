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
std::uniform_int_distribution<> uniform01(0, 1);
auto rng_01 = std::bind(uniform01, engine);
std::uniform_int_distribution<> uniform(0, CHROMOSOME_SIZE - 1);
auto rng_size = std::bind(uniform, engine);

template<typename population> void initialize(population &);
template<typename population> void mutate(population &);
template<typename population> void crossover(population &);
template<typename population> std::size_t evaluate(population &);

template<typename population> int common_main(int argc, char *argv[])
{
    std::size_t step = std::numeric_limits<std::size_t>::max(), value = 0;

    if (argc > 1)
        step = std::atoi(argv[1]);

    std::cout << "step = " << step << '\n';

    if (step == 0)
        return value;

    population p(POPULATION_SIZE);

    if (step == 1)
        return value;

    initialize(p);

    if (step == 2)
        return value;

    mutate(p);

    if (step == 3)
        return value;

    crossover(p);

    if (step == 4)
        return value;

    value = evaluate(p);

    return value;
}

#endif // common_h
