#include "rng.h"

#include <chrono>
#include <iostream>
#include <random>
#include <string_view>
#include <tuple>
#include <unistd.h>

using namespace std::chrono_literals;
using namespace std::literals;

template<typename F> std::size_t test(F f, const char *engine)
{
    const std::size_t N = 1'000'000, R = 100;

    std::size_t r = 0;
    auto min = 999'999'999'999ns;

    for (std::size_t i = 0; i < R; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        for (std::size_t j = 0; j < N; ++j)
            r ^= f();
        auto stop = std::chrono::high_resolution_clock::now();
        min = std::min(min, (stop - start));
    }

    return r;
}

std::string_view parser(int argc, char **argv)
{
    std::string_view engine = "default_random_engine";
    int option = 0;

    while ((option = getopt(argc, argv, "e:h")) != -1)
        switch (option)
        {
            case 'e':
                engine = std::string_view(optarg);
                break;
            case 'h':
                std::cout << argv[0] << " options:\n"
                          << "\t-e: random engine          (optional)\n"
                          << "\t    default_random_engine\n"
                          << "\t    knuth_b\n"
                          << "\t    minstd_rand0\n"
                          << "\t    minstd_rand\n"
                          << "\t    mt19937\n"
                          << "\t    mt19937_64\n"
                          << "\t    mt11213b\n"
                          << "\t    rand\n"
                          << "\t    ranlux24_base\n"
                          << "\t    ranlux48_base\n"
                          << "\t    ranlux24\n"
                          << "\t    ranlux48\n"
                          << "\t    xoroshiro128p\n"
                          << "\t    xoshiro256p\n"
                          << "\t-h: help                   (optional)\n"
                          << "\t-s: random seed            (optional)\n";
                exit(EXIT_SUCCESS);
                break;
        }

    return engine;
}

int main(int argc, char *argv[])
{
    auto engine = parser(argc, argv);

    if (engine == "default_random_engine")
        return test(std::default_random_engine(), engine.data());
    else if (engine == "knuth_b")
        return test(std::knuth_b(), engine.data());
    else if (engine == "minstd_rand0")
        return test(std::minstd_rand0(), engine.data());
    else if (engine == "minstd_rand")
        return test(std::minstd_rand(), engine.data());
    else if (engine == "mt19937")
        return test(std::mt19937(), engine.data());
    else if (engine == "mt19937_64")
        return test(std::mt19937_64(), engine.data());
    else if (engine == "mt11213b")
        return test(mt11213b(), engine.data());
    else if (engine == "rand")
        return test(rand_t(), engine.data());
    else if (engine == "ranlux24_base")
        return test(std::ranlux24_base(), engine.data());
    else if (engine == "ranlux48_base")
        return test(std::ranlux48_base(), engine.data());
    else if (engine == "ranlux24")
        return test(std::ranlux24(), engine.data());
    else if (engine == "ranlux48")
        return test(std::ranlux48(), engine.data());
    else if (engine == "xoroshiro128p")
        return test(xoroshiro128p(), engine.data());
    else if (engine == "xoshiro256p")
        return test(xoshiro256p(), engine.data());
    else
    {
        std::cerr << "Unknown engine: " << engine << '\n';
        return EXIT_FAILURE;
    }
}
