#include "rng.h"

#include <chrono>
#include <iostream>
#include <random>
#include <string_view>
#include <unistd.h>

template<typename Engine>
typename Engine::result_type test(Engine engine = Engine())
{
    const std::size_t N = 100'000'000;
    typename Engine::result_type r = 0;

    for (std::size_t j = 0; j < N; ++j)
        r ^= engine();

    return r;
}

std::string_view parser(int argc, char **argv)
{
    std::string_view engine = "minstd_rand0";
    int option = 0;

    while ((option = getopt(argc, argv, "e:h")) != -1)
        switch (option)
        {
            case 'e':
                engine = std::string_view(optarg);
                break;
            case 'h':
                std::cout << argv[0] << " options:\n"
                          << "\t-e: random engine             (optional)\n"
                          << "\t    knuth_b\n"
                          << "\t    minstd_rand0 (default_random_engine)\n"
                          << "\t    minstd_rand\n"
                          << "\t    mt19937\n"
                          << "\t    mt19937_64\n"
                          << "\t    mt11213b\n"
                          << "\t    rand\n"
                          << "\t    ranlux24_base\n"
                          << "\t    ranlux48_base\n"
                          << "\t    ranlux24\n"
                          << "\t    ranlux48\n"
                          << "\t    romutrio32\n"
                          << "\t    romutrio\n"
                          << "\t    xoroshiro128+\n"
                          << "\t    xoshiro256+\n"
                          << "\t-h: this help                (optional)\n";
                exit(EXIT_SUCCESS);
                break;
        }

    return engine;
}

int main(int argc, char *argv[])
{
    auto engine = parser(argc, argv);

    if (engine == "knuth_b")
        return test<std::knuth_b>();
    else if (engine == "minstd_rand0")
        return test<std::minstd_rand0>();
    else if (engine == "minstd_rand")
        return test<std::minstd_rand>();
    else if (engine == "mt19937")
        return test<std::mt19937>();
    else if (engine == "mt19937_64")
        return test<std::mt19937_64>();
    else if (engine == "mt11213b")
        return test<mt11213b>();
    else if (engine == "rand")
        return test<rand_t>();
    else if (engine == "ranlux24_base")
        return test<std::ranlux24_base>();
    else if (engine == "ranlux48_base")
        return test<std::ranlux48_base>();
    else if (engine == "ranlux24")
        return test<std::ranlux24>();
    else if (engine == "ranlux48")
        return test<std::ranlux48>();
    else if (engine == "romutrio32")
        return test<romutrio32>();
    else if (engine == "romutrio")
        return test<romutrio>();
    else if (engine == "xoroshiro128+")
        return test<xoroshiro128p>();
    else if (engine == "xoshiro256+")
        return test<xoshiro256p>();
    else
    {
        std::cerr << "Unknown engine: " << engine << '\n';
        return EXIT_FAILURE;
    }
}
