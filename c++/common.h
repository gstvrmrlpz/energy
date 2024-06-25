#ifndef common_h
#define common_h 1

#include <functional>
#include <random>
#include <vector>

const std::size_t CHROMOSOME_SIZE = 2048, POPULATION_SIZE = 40'000;

class chromosome;
using population = std::vector<chromosome>;

std::random_device device;
std::default_random_engine engine(device());
std::bernoulli_distribution bernoulli(0.5);
auto rng_01 = std::bind(bernoulli, engine);
std::uniform_int_distribution<> uniform(0, CHROMOSOME_SIZE - 1);
auto rng_size = std::bind(uniform, engine);

void initialize(population &);
void mutate(population &);
void crossover(population &);
std::size_t evaluate(population &);

int common_main()
{
    population p(POPULATION_SIZE);

    initialize(p);
    mutate(p);
    crossover(p);
    return evaluate(p);
}

#endif // common_h
