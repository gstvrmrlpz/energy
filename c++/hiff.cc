#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>

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

constexpr char T(const std::string_view &s)
{
    switch (s.size())
    {
        case 1:
            return s.front();
            break;
        case 2:
            return t(s.front(), s.back());
            break;
        default:
            return t(T(s.substr(0, s.size() / 2)),
                     T(s.substr(s.size() / 2, s.size())));
            break;
    }
}

// T test
static_assert(T("0") == '0');
static_assert(T("1") == '1');
static_assert(T("-") == '-');
static_assert(T("00") == '0');
static_assert(T("01") == '-');
static_assert(T("10") == '-');
static_assert(T("11") == '1');
static_assert(T("0-") == '-');
static_assert(T("-1") == '-');
static_assert(T("0000") == '0');
static_assert(T("0-00") == '-');
static_assert(T("0011") == '-');
static_assert(T("11-1") == '-');
static_assert(T("1111") == '1');

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

constexpr unsigned HIFF(const std::string_view &s)
{
    if (s.size() == 1)
        return 1;
    else
        return s.size() * f(T(s)) + HIFF(s.substr(0, s.size() / 2)) +
               HIFF(s.substr(s.size() / 2));
}

// HIFF test

static_assert(HIFF("0") == 1);
static_assert(HIFF("1") == 1);
static_assert(HIFF("00") == 4);
static_assert(HIFF("01") == 2);
static_assert(HIFF("10") == 2);
static_assert(HIFF("11") == 4);
static_assert(HIFF("0011") == 8);
static_assert(HIFF("1111") == 12);
static_assert(HIFF("10110000") == 18);
static_assert(HIFF("00001101") == 18);

//------------------------------------------------------------------------

void next(std::string &s)
{
    for (auto &i : std::views::reverse(s))
        if (i == '0')
        {
            i = '1';
            break;
        }
        else
        {
            i = '0';
            continue;
        }
}

//------------------------------------------------------------------------

int main()
{
    const std::size_t L = 8, N = 3;

    std::string s[N] = {
        std::string(L, '0'), std::string(L, '1'), std::string(L, '-')};

    for (std::size_t n = 1; n < 16; n <<= 1)
    {
        std::string last = std::string(n, '1');
        for (std::string s = std::string(n, '0'); s != last; next(s))
            std::cout << "HIFF(" << s << ") = " << HIFF(s) << std::endl;
        std::cout << "HIFF(" << last << ") = " << HIFF(last) << std::endl;
    }
}
