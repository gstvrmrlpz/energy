#ifndef common_h
#define common_h 1

//------------------------------------------------------------------------

#include "hiff.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <random>
#include <tuple>
#include <unistd.h>

//------------------------------------------------------------------------

const std::size_t POPULATION_SIZE = 40'000;

//------------------------------------------------------------------------

std::random_device device;
std::default_random_engine engine(device());

std::bernoulli_distribution bernoulli;
auto rng_ft = std::bind(std::ref(bernoulli), std::ref(engine));

std::uniform_int_distribution<char> uniform_char('0', '1');
auto rng_01 = std::bind(std::ref(uniform_char), std::ref(engine));

std::uniform_int_distribution<> uniform_int;
auto rng_size = std::bind(std::ref(uniform_int), std::ref(engine));

//------------------------------------------------------------------------

template<typename chromosome> void initialize(chromosome &, std::size_t);
template<typename chromosome> void mutate(chromosome &);
template<typename chromosome> void crossover(chromosome &, chromosome &);
template<typename chromosome> std::size_t onemax(const chromosome &);
template<typename chromosome> std::size_t hiff(const chromosome &);

//------------------------------------------------------------------------

std::tuple<std::size_t, char> parser(int argc, char *argv[])
{
    int option = 0;
    std::size_t size = 0;
    char work = ' ';

    while ((option = getopt(argc, argv, "hr:s:w:")) != -1)
        switch (option)
        {
            case 'h':
                std::cout << argv[0] << " options:\n"
                          << "\t-h: help                   (optional)\n"
                          << "\t-r: random seed            (optional)\n"
                          << "\t-s: chromosome size        (required)\n"
                          << "\t-w: work stage ∈ {i, g, h} (required)\n"
                          << "\t    i for init only\n"
                          << "\t    g for genetic algorithm\n"
                          << "\t    h for HIFF\n";
                exit(EXIT_SUCCESS);
                break;
            case 'r':
                engine.seed(atoi(optarg));
                break;
            case 's':
                size = atoi(optarg);
                uniform_int = std::uniform_int_distribution<>(0, size - 1);
                break;
            case 'w':
                work = optarg[0];
                break;
        }

    if (work != 'i' && work != 'g' && work != 'h')
    {
        std::cout << "-w option must be one of {i, g, h}\n";
        exit(EXIT_FAILURE);
    }

    return {size, work};
}

//------------------------------------------------------------------------

template<typename chromosome> int common_main(int argc, char *argv[])
{
    auto [size, work] = parser(argc, argv);

    std::size_t value = 0;

    std::vector<chromosome> population(POPULATION_SIZE);

    for (auto &i : population)
        initialize(i, size);

    switch (work)
    {
        case 'i':
            break;
        case 'g':
            for (std::size_t i = 0; i < population.size(); i += 2)
            {
                mutate(population[i]);
                mutate(population[i + 1]);
                crossover(population[i], population[i + 1]);
                value += onemax(population[i]) + onemax(population[i + 1]);
            }
            break;
        case 'h':
            for (const auto &i : population)
                value += hiff(i);
            break;
    }

    return value;
}

//------------------------------------------------------------------------

#endif // common_h
