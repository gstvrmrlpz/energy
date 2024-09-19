#ifndef common_h
#define common_h 1

//------------------------------------------------------------------------

#include "hiff.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <random>
#include <string_view>
#include <tuple>
#include <unistd.h>
#include <unordered_map>

//------------------------------------------------------------------------

using namespace std::literals;

//------------------------------------------------------------------------

const std::size_t POPULATION_SIZE = 40'000;

//------------------------------------------------------------------------

std::tuple<std::default_random_engine,
           std::knuth_b,
           std::minstd_rand0,
           std::minstd_rand,
           std::mt19937,
           std::mt19937_64,
           std::ranlux24,
           std::ranlux48>
    engines;

std::unordered_map<std::string_view, char> engine_map = {
    {"default_random_engine"sv, 1},
    {"knuth_b"sv, 2},
    {"minstd_rand0"sv, 3},
    {"minstd_rand"sv, 4},
    {"mt19937"sv, 5},
    {"mt19937_64"sv, 6},
    {"ranlux24"sv, 7},
    {"ranlux48"sv, 8}}; // don't use value 0, reserve for unknowns

std::function<bool()> rng_ft =
    std::bind(std::bernoulli_distribution(), std::get<0>(engines));
std::function<char()> rng_01 = std::bind(
    std::uniform_int_distribution<char>('0', '1'), std::get<0>(engines));
std::function<std::size_t()> rng_size =
    std::bind(std::uniform_int_distribution<>(), std::get<0>(engines));

//------------------------------------------------------------------------

template<typename chromosome> void initialize(chromosome &, std::size_t);
template<typename chromosome> void mutate(chromosome &);
template<typename chromosome> void crossover(chromosome &, chromosome &);
template<typename chromosome> std::size_t onemax(const chromosome &);
template<typename chromosome> std::size_t hiff(const chromosome &);

//------------------------------------------------------------------------

std::tuple<std::size_t, char> parser(int argc, char *argv[])
{
    std::string_view engine = "default_random_engine";
    int option = 0;
    std::size_t size = 0, seed = std::random_device()();
    char work = ' ';

    while ((option = getopt(argc, argv, "e:hr:s:w:")) != -1)
        switch (option)
        {
            case 'e':
                engine = std::string_view(optarg);
                break;
            case 'h':
                std::cout << argv[0] << " options:\n"
                          << "\t-e: random engine          (optional)\n"
                          << "\t    default_random_engine   (default)\n"
                          << "\t    knuth_b\n"
                          << "\t    minstd_rand0\n"
                          << "\t    minstd_rand\n"
                          << "\t    mt19937\n"
                          << "\t    mt19937_64\n"
                          << "\t    ranlux24\n"
                          << "\t    ranlux48\n"
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
                seed = atoi(optarg);
                break;
            case 's':
                size = atoi(optarg);
                break;
            case 'w':
                work = optarg[0];
                break;
        }

    if (size == 0)
    {
        std::cout << "-s option is required\n";
        exit(EXIT_FAILURE);
    }

    if (work != 'i' && work != 'g' && work != 'h')
    {
        std::cout << "-w option must be one of {i, g, h}\n";
        exit(EXIT_FAILURE);
    }

    // random seed
    std::apply([&](auto &...e) { (e.seed(seed), ...); }, engines);

    // random engine
    switch (engine_map[engine])
    {
        case 1:
            rng_ft = std::bind(std::bernoulli_distribution(),
                               std::get<0>(engines));
            rng_01 =
                std::bind(std::uniform_int_distribution<char>('0', '1'),
                          std::get<0>(engines));
            rng_size =
                std::bind(std::uniform_int_distribution<>(0, size - 1),
                          std::get<0>(engines));
            break;

        case 2:
            rng_ft = std::bind(std::bernoulli_distribution(),
                               std::get<1>(engines));
            rng_01 =
                std::bind(std::uniform_int_distribution<char>('0', '1'),
                          std::get<1>(engines));
            rng_size =
                std::bind(std::uniform_int_distribution<>(0, size - 1),
                          std::get<1>(engines));
            break;

        case 3:
            rng_ft = std::bind(std::bernoulli_distribution(),
                               std::get<2>(engines));
            rng_01 =
                std::bind(std::uniform_int_distribution<char>('0', '1'),
                          std::get<2>(engines));
            rng_size =
                std::bind(std::uniform_int_distribution<>(0, size - 1),
                          std::get<2>(engines));
            break;

        case 4:
            rng_ft = std::bind(std::bernoulli_distribution(),
                               std::get<3>(engines));
            rng_01 =
                std::bind(std::uniform_int_distribution<char>('0', '1'),
                          std::get<3>(engines));
            rng_size =
                std::bind(std::uniform_int_distribution<>(0, size - 1),
                          std::get<3>(engines));
            break;

        case 5:
            rng_ft = std::bind(std::bernoulli_distribution(),
                               std::get<4>(engines));
            rng_01 =
                std::bind(std::uniform_int_distribution<char>('0', '1'),
                          std::get<4>(engines));
            rng_size =
                std::bind(std::uniform_int_distribution<>(0, size - 1),
                          std::get<4>(engines));
            break;

        case 6:
            rng_ft = std::bind(std::bernoulli_distribution(),
                               std::get<5>(engines));
            rng_01 =
                std::bind(std::uniform_int_distribution<char>('0', '1'),
                          std::get<5>(engines));
            rng_size =
                std::bind(std::uniform_int_distribution<>(0, size - 1),
                          std::get<5>(engines));
            break;

        case 7:
            rng_ft = std::bind(std::bernoulli_distribution(),
                               std::get<6>(engines));
            rng_01 =
                std::bind(std::uniform_int_distribution<char>('0', '1'),
                          std::get<6>(engines));
            rng_size =
                std::bind(std::uniform_int_distribution<>(0, size - 1),
                          std::get<6>(engines));
            break;

        case 8:
            rng_ft = std::bind(std::bernoulli_distribution(),
                               std::get<7>(engines));
            rng_01 =
                std::bind(std::uniform_int_distribution<char>('0', '1'),
                          std::get<7>(engines));
            rng_size =
                std::bind(std::uniform_int_distribution<>(0, size - 1),
                          std::get<7>(engines));
            break;

        default:
            std::cout << "Invalid random engine " << engine << "\n";
            exit(EXIT_FAILURE);
            break;
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
