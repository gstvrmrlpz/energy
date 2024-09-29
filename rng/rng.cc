#include "rng.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

using namespace std::chrono_literals;

std::size_t r = 0;

template<typename F> void test(F f, const char *engine)
{
    const std::size_t N = 1'000'000, R = 1000;

    auto min = 999'999'999'999ns;

    for (std::size_t i = 0; i < R; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        for (std::size_t j = 0; j < N; ++j)
            r ^= f();
        auto stop = std::chrono::high_resolution_clock::now();
        min = std::min(min, (stop - start));
    }

    std::cout << std::setw(8) << min.count() << " ns " << engine << "\n";
}

int main()
{
    test(std::default_random_engine{}, "std::default_random_engine");
    test(std::minstd_rand0{}, "std::minstd_rand0");
    test(std::minstd_rand{}, "std::minstd_rand");
    test(std::mt19937{}, "std::mt19937");
    test(std::mt19937_64{}, "std::mt19937_64");
    test(std::ranlux24_base{}, "std::ranlux24_base");
    test(std::ranlux48_base{}, "std::ranlux48_base");
    test(std::ranlux24{}, "std::ranlux24");
    test(std::ranlux48{}, "std::ranlux48");
    test(std::knuth_b{}, "std::knuth_b");

    test(rand_t{}, "rand_t");
    test(mt11213b{}, "mt11213b");

    test(xoroshiro128p{}, "xoroshiro128");
    test(xoshiro256p{}, "xoshiro256");

    return r;
}
