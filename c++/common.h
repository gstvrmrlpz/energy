#ifndef common_h
#define common_h 1

#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <random>

//------------------------------------------------------------------------

const std::size_t CHROMOSOME_SIZE = 1024, POPULATION_SIZE = 40'000;

//------------------------------------------------------------------------

std::random_device device;
std::default_random_engine engine(device());

std::bernoulli_distribution bernoulli;
auto rng_ft = std::bind(bernoulli, engine);

std::uniform_int_distribution<char> uniform_char('0', '1');
auto rng_01 = std::bind(uniform_char, engine);

std::uniform_int_distribution<> uniform(0, CHROMOSOME_SIZE - 1);
auto rng_size = std::bind(uniform, engine);

//------------------------------------------------------------------------

constexpr unsigned f(const char c)
{
    if (c == '-')
        return 0;
    else
        return 1;
}

// f test
static_assert(f('0') == 1);
static_assert(f('1') == 1);
static_assert(f('-') == 0);

//------------------------------------------------------------------------

constexpr char t(const char left, const char right)
{
    if (left == right)
        return left;
    else
        return '-';
}

// t test
static_assert(t('0', '0') == '0');
static_assert(t('0', '1') == '-');
static_assert(t('1', '0') == '-');
static_assert(t('1', '1') == '1');
static_assert(t('0', '-') == '-');
static_assert(t('-', '1') == '-');

//------------------------------------------------------------------------

template<typename chromosome> void initialize(chromosome &);
template<typename chromosome> void mutate(chromosome &);
template<typename chromosome> void crossover(chromosome &, chromosome &);
template<typename chromosome> std::size_t evaluate(const chromosome &);

//------------------------------------------------------------------------

template<typename chromosome> int common_main(int argc, char *argv[])
{
    std::size_t value = 0;

    std::vector<chromosome> population(POPULATION_SIZE);

    for (std::size_t i = 0; i < population.size(); ++i)
        initialize(population[i]);

    if (argc != 1)
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

//------------------------------------------------------------------------

#endif // common_h
