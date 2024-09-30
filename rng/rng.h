//-----------------------------------------------------------------------------
// rng.h: source wikipedia & https://rng.di.unimi.it/
//-----------------------------------------------------------------------------

#ifndef RNG_H
#define RNG_H 1

//-----------------------------------------------------------------------------

#include <bit>
#include <cassert>
#include <initializer_list>
#include <limits>
#include <random>
#include <type_traits>

//-----------------------------------------------------------------------------

#ifndef __cpp_lib_bitops // macro for pre C++20 compilers
namespace std
{
    template<class T> constexpr T rotl(T x, int k)
    {
        return (x << k) | (x >> (64 - k));
    }
} // namespace std
#endif

//-----------------------------------------------------------------------------

template<class T> class splitmix
{
public:
    typedef T result_type;

    splitmix(result_type seed = std::random_device()())
        : state(seed)
    {
    }

protected:
    result_type state;
};

class splitmix32 : public splitmix<uint32_t>
{
public:
    const result_type phi = 0x9e3779b9;

    splitmix32(result_type seed = std::random_device()())
        : splitmix(seed)
    {
    }

    result_type operator()()
    {
        result_type z = (state += phi);
        z = (z ^ (z >> 16)) * 0x85ebca6b;
        z = (z ^ (z >> 13)) * 0xc2b2ae35;
        return z ^ (z >> 16);
    }
};

class splitmix64
    : public splitmix<uint64_t> // https://xoshiro.di.unimi.it/splitmix64.c
{
public:
    const result_type phi = 0x9e3779b97f4a7c15;

    splitmix64(result_type seed = std::random_device()())
        : splitmix(seed)
    {
    }

    result_type operator()()
    {
        result_type z = (state += phi);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }
};

//-----------------------------------------------------------------------------

struct rand_t
{
    rand_t(int seed = std::random_device()()) { srand(seed); }
    int operator()() { return std::rand(); }
    int min() { return 0; }
    int max() { return RAND_MAX; }
};

//-----------------------------------------------------------------------------

template<class T, std::size_t state_size = 1> class engine_base
{
public:
    typedef T result_type;

    engine_base(result_type seed = std::random_device()())
    {
        splitmix64 s(seed);
        for (std::size_t i = 0; i < state_size; ++i)
            state[i] = s();
    }

    engine_base(std::initializer_list<result_type> il)
    {
        assert(il.size() == state_size); // wrong number of seeds?
        std::copy(il.begin(), il.end(), state);
    }

    static constexpr T min()
    {
        return std::numeric_limits<result_type>::min();
    }

    static constexpr T max()
    {
        return std::numeric_limits<result_type>::max();
    }

protected:
    result_type state[state_size];
};

//-----------------------------------------------------------------------------

class xorshift32 : public engine_base<uint32_t>
{
public:
    xorshift32() : engine_base<uint32_t>{1234} {}

    /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
    result_type operator()()
    {
        state[0] ^= state[0] << 13;
        state[0] ^= state[0] >> 17;
        state[0] ^= state[0] << 5;
        return state[0];
    }
};

//-----------------------------------------------------------------------------

class xorshift64 : public engine_base<uint64_t>
{
public:
    xorshift64() : engine_base<uint64_t>{1234} {}

    /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
    result_type operator()()
    {
        state[0] ^= state[0] << 13;
        state[0] ^= state[0] >> 7;
        state[0] ^= state[0] << 17;
        return state[0];
    }
};

//-----------------------------------------------------------------------------

class xorshift64s : engine_base<uint64_t>
{
public:
    result_type operator()()
    {
        result_type x = state[0]; // state must be nonzero
        x ^= x >> 12;             // a
        x ^= x << 25;             // b
        x ^= x >> 27;             // c
        state[0] = x;
        return x * 0x2545F4914F6CDD1DULL;
    }
};

//-----------------------------------------------------------------------------

class xorshift96 : public engine_base<uint64_t, 3>
{
public:
    xorshift96()
        : engine_base<uint64_t, 3>{123456789, 362436069, 521288629}
    {
    }

    result_type operator()()
    {
        state[0] ^= state[0] << 16;
        state[0] ^= state[0] >> 5;
        state[0] ^= state[0] << 1;
        result_type t = state[0];
        state[0] = state[1];
        state[1] = state[2];
        state[2] = t ^ state[0] ^ state[1];
        return state[2];
    }
};

//-----------------------------------------------------------------------------

class xorshift128 : public engine_base<uint64_t, 4>
{
public:
    xorshift128()
        : engine_base<uint64_t, 4>{
              123456789, 362436069, 521288629, 88675123}
    {
    }

    result_type operator()()
    {
        result_type t = state[0] ^ (state[0] << 11);
        state[0] = state[1];
        state[1] = state[2];
        state[2] = state[3];
        state[3] ^= (state[3] >> 19) ^ (t ^ (t >> 8));
        return state[3];
    }
};

//-----------------------------------------------------------------------------

class xorshift128p : public engine_base<uint64_t, 2>
{
public:
    xorshift128p() : engine_base<uint64_t, 2>{123456789, 362436069} {}

    result_type operator()()
    {
        result_type t = state[0];
        result_type const s = state[1];
        state[0] = s;
        t ^= t << 23; // a
        t ^= t >> 18; // b -- Again, the shifts and the multipliers are
        t ^= s ^ (s >> 5); // c
        state[1] = t;
        return t + s;
    }
};

//-----------------------------------------------------------------------------

class xorwow : public engine_base<uint32_t, 5>
{
public:
    xorwow() : engine_base<uint32_t, 5>(), counter(0) {}

    // Algorithm "xorwow" from p. 5 of Marsaglia, "Xorshift RNGs"
    result_type operator()()
    {
        result_type t = state[4];
        result_type s = state[0]; // Perform a contrived 32-bit shift
        state[4] = state[3];
        state[3] = state[2];
        state[2] = state[1];
        state[1] = s;
        t ^= t >> 2;
        t ^= t << 1;
        t ^= s ^ (s << 4);
        state[0] = t;
        counter += 362437;
        return t + counter;
    }

private:
    result_type counter;
};

//-----------------------------------------------------------------------------

class xorshift1024s : public engine_base<uint64_t, 16>
{
public:
    xorshift1024s() : engine_base<uint64_t, 16>(), index(0) {}

    result_type operator()()
    {
        result_type const s = state[index++];
        result_type t = state[index &= 15];
        t ^= t << 31;       // a
        t ^= t >> 11;       // b
        t ^= s ^ (s >> 30); // c
        state[index] = t;
        return t * 1181783497276652981ULL;
    }

private:
    std::size_t index;
};

//-----------------------------------------------------------------------------

class xoshiro256p : public engine_base<uint64_t, 4> // zig's xoshiro256
{
public:
    result_type operator()()
    {
        const result_type result = state[0] + state[3];
        const result_type t = state[1] << 17;
        state[2] ^= state[0];
        state[3] ^= state[1];
        state[1] ^= state[2];
        state[0] ^= state[3];
        state[2] ^= t;
        state[3] = std::rotl(state[3], 45);
        return result;
    }
};

//-----------------------------------------------------------------------------

class xoshiro256ss : public engine_base<uint64_t, 4>
{
public:
    result_type operator()()
    {
        const result_type result = std::rotl(state[1] * 5, 7) * 9;
        const result_type t = state[1] << 17;
        state[2] ^= state[0];
        state[3] ^= state[1];
        state[1] ^= state[2];
        state[0] ^= state[3];
        state[2] ^= t;
        state[3] = std::rotl(state[3], 45);
        return result;
    }
};

//-----------------------------------------------------------------------------

class xoshiro256pp : public engine_base<uint64_t, 4>
{
public:
    result_type operator()()
    {
        const result_type result =
            std::rotl(state[0] + state[3], 23) + state[0];
        const result_type t = state[1] << 17;
        state[2] ^= state[0];
        state[3] ^= state[1];
        state[1] ^= state[2];
        state[0] ^= state[3];
        state[2] ^= t;
        state[3] = std::rotl(state[3], 45);
        return result;
    }
};

//-----------------------------------------------------------------------------

class xoshiro512pp : public engine_base<uint64_t, 8>
{
public:
    result_type operator()()
    {
        const uint64_t result =
            std::rotl(state[0] + state[2], 17) + state[2];
        const uint64_t t = state[1] << 11;
        state[2] ^= state[0];
        state[5] ^= state[1];
        state[1] ^= state[2];
        state[7] ^= state[3];
        state[3] ^= state[4];
        state[4] ^= state[5];
        state[0] ^= state[6];
        state[6] ^= state[7];
        state[6] ^= t;
        state[7] = std::rotl(state[7], 21);
        return result;
    }
};

//-----------------------------------------------------------------------------

class xoshiro512ss : public engine_base<uint64_t, 8>
{
public:
    result_type operator()()
    {
        const uint64_t result = std::rotl(state[1] * 5, 7) * 9;
        const uint64_t t = state[1] << 11;
        state[2] ^= state[0];
        state[5] ^= state[1];
        state[1] ^= state[2];
        state[7] ^= state[3];
        state[3] ^= state[4];
        state[4] ^= state[5];
        state[0] ^= state[6];
        state[6] ^= state[7];
        state[6] ^= t;
        state[7] = std::rotl(state[7], 21);
        return result;
    }
};

//-----------------------------------------------------------------------------

class xoshiro512p : public engine_base<uint64_t, 8>
{
public:
    result_type operator()()
    {
        const uint64_t result = state[0] + state[2];
        const uint64_t t = state[1] << 11;
        state[2] ^= state[0];
        state[5] ^= state[1];
        state[1] ^= state[2];
        state[7] ^= state[3];
        state[3] ^= state[4];
        state[4] ^= state[5];
        state[0] ^= state[6];
        state[6] ^= state[7];
        state[6] ^= t;
        state[7] = std::rotl(state[7], 21);
        return result;
    }
};

//-----------------------------------------------------------------------------

class xoroshiro128p : public engine_base<uint64_t, 2>
{
public:
    result_type operator()()
    {
        const result_type s0 = state[0];
        result_type s1 = state[1];
        const result_type result = s0 + s1;
        s1 ^= s0;
        state[0] = std::rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
        state[1] = std::rotl(s1, 37);                   // c
        return result;
    }
};

//-----------------------------------------------------------------------------

class xoroshiro128pp : public engine_base<uint64_t, 2>
{
public:
    result_type operator()()
    {
        const result_type s0 = state[0];
        result_type s1 = state[1];
        const result_type result = std::rotl(s0 + s1, 17) + s0;
        s1 ^= s0;
        state[0] = std::rotl(s0, 49) ^ s1 ^ (s1 << 21); // a, b
        state[1] = std::rotl(s1, 28);                   // c
        return result;
    }
};

//-----------------------------------------------------------------------------

class xoroshiro128ss : public engine_base<uint64_t, 2>
{
public:
    result_type operator()()
    {
        const uint64_t s0 = state[0];
        uint64_t s1 = state[1];
        const uint64_t result = std::rotl(s0 * 5, 7) * 9;
        s1 ^= s0;
        state[0] = std::rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
        state[1] = std::rotl(s1, 37);                   // c
        return result;
    }
};

//-----------------------------------------------------------------------------

typedef std::mersenne_twister_engine<uint32_t,
                                     32,
                                     351,
                                     175,
                                     19,
                                     0xccab8ee7,
                                     11,
                                     0xffffffff,
                                     7,
                                     0x31b6ab00,
                                     15,
                                     0xffe50000,
                                     17,
                                     1812433253>
    mt11213b;

//-----------------------------------------------------------------------------

/*
pub fn next(self: *Xoroshiro128) u64 {
    const s0 = self.s[0];
    var s1 = self.s[1];
    const r = s0 +% s1;

    s1 ^= s0;
    self.s[0] = math.rotl(u64, s0, @as(u8, 55)) ^ s1 ^ (s1 << 14);
    self.s[1] = math.rotl(u64, s1, @as(u8, 36));

    return r;
}

pub fn next(self: *Xoshiro256) u64 {
    const r = math.rotl(u64, self.s[0] +% self.s[3], 23) +% self.s[0];

    const t = self.s[1] << 17;

    self.s[2] ^= self.s[0];
    self.s[3] ^= self.s[1];
    self.s[1] ^= self.s[2];
    self.s[0] ^= self.s[3];

    self.s[2] ^= t;

    self.s[3] = math.rotl(u64, self.s[3], 45);

    return r;
}
*/

//-----------------------------------------------------------------------------

#endif // RNG_H
