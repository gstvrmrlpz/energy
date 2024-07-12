#ifndef hiff_h
#define hiff_h 1

#include <string_view>
#include <vector>

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
static_assert(t('-', '-') == '-');
static_assert(t('-', '0') == '-');
static_assert(t('-', '1') == '-');
static_assert(t('0', '-') == '-');
static_assert(t('0', '0') == '0');
static_assert(t('0', '1') == '-');
static_assert(t('1', '-') == '-');
static_assert(t('1', '0') == '-');
static_assert(t('1', '1') == '1');

//------------------------------------------------------------------------
// string_view
//------------------------------------------------------------------------

constexpr char T(std::string_view s)
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

static_assert(T("--") == '-');
static_assert(T("-0") == '-');
static_assert(T("-1") == '-');
static_assert(T("0-") == '-');
static_assert(T("00") == '0');
static_assert(T("01") == '-');
static_assert(T("1-") == '-');
static_assert(T("10") == '-');
static_assert(T("11") == '1');

static_assert(T("----") == '-');
static_assert(T("---0") == '-');
static_assert(T("---1") == '-');
static_assert(T("--0-") == '-');
static_assert(T("--00") == '-');
static_assert(T("--01") == '-');
static_assert(T("--1-") == '-');
static_assert(T("--10") == '-');
static_assert(T("--11") == '-');
static_assert(T("-0--") == '-');
static_assert(T("-0-0") == '-');
static_assert(T("-0-1") == '-');
static_assert(T("-00-") == '-');
static_assert(T("-000") == '-');
static_assert(T("-001") == '-');
static_assert(T("-01-") == '-');
static_assert(T("-010") == '-');
static_assert(T("-011") == '-');
static_assert(T("-1--") == '-');
static_assert(T("-1-0") == '-');
static_assert(T("-1-1") == '-');
static_assert(T("-10-") == '-');
static_assert(T("-100") == '-');
static_assert(T("-101") == '-');
static_assert(T("-11-") == '-');
static_assert(T("-110") == '-');
static_assert(T("-111") == '-');
static_assert(T("0---") == '-');
static_assert(T("0--0") == '-');
static_assert(T("0--1") == '-');
static_assert(T("0-0-") == '-');
static_assert(T("0-00") == '-');
static_assert(T("0-01") == '-');
static_assert(T("0-1-") == '-');
static_assert(T("0-10") == '-');
static_assert(T("0-11") == '-');
static_assert(T("00--") == '-');
static_assert(T("00-0") == '-');
static_assert(T("00-1") == '-');
static_assert(T("000-") == '-');
static_assert(T("0000") == '0');
static_assert(T("0001") == '-');
static_assert(T("001-") == '-');
static_assert(T("0010") == '-');
static_assert(T("0011") == '-');
static_assert(T("01--") == '-');
static_assert(T("01-0") == '-');
static_assert(T("01-1") == '-');
static_assert(T("010-") == '-');
static_assert(T("0100") == '-');
static_assert(T("0101") == '-');
static_assert(T("011-") == '-');
static_assert(T("0110") == '-');
static_assert(T("0111") == '-');
static_assert(T("1---") == '-');
static_assert(T("1--0") == '-');
static_assert(T("1--1") == '-');
static_assert(T("1-0-") == '-');
static_assert(T("1-00") == '-');
static_assert(T("1-01") == '-');
static_assert(T("1-1-") == '-');
static_assert(T("1-10") == '-');
static_assert(T("1-11") == '-');
static_assert(T("10--") == '-');
static_assert(T("10-0") == '-');
static_assert(T("10-1") == '-');
static_assert(T("100-") == '-');
static_assert(T("1000") == '-');
static_assert(T("1001") == '-');
static_assert(T("101-") == '-');
static_assert(T("1010") == '-');
static_assert(T("1011") == '-');
static_assert(T("11--") == '-');
static_assert(T("11-0") == '-');
static_assert(T("11-1") == '-');
static_assert(T("110-") == '-');
static_assert(T("1100") == '-');
static_assert(T("1101") == '-');
static_assert(T("111-") == '-');
static_assert(T("1110") == '-');
static_assert(T("1111") == '1');

//------------------------------------------------------------------------

constexpr std::size_t HIFF(std::string_view s)
{
    if (s.size() == 1)
        return 1;
    else
        return s.size() * f(T(s)) + HIFF(s.substr(0, s.size() / 2)) +
               HIFF(s.substr(s.size() / 2));
}

static_assert(HIFF("0") == 1);
static_assert(HIFF("1") == 1);

static_assert(HIFF("00") == 4);
static_assert(HIFF("01") == 2);
static_assert(HIFF("10") == 2);
static_assert(HIFF("11") == 4);

static_assert(HIFF("0011") == 8);
static_assert(HIFF("1111") == 12);

static_assert(HIFF("00000000") == 32 && HIFF("11111111") == 32);
static_assert(HIFF("10110000") == 18 && HIFF("00001101") == 18);
static_assert(HIFF("01010101") == 8 && HIFF("10101010") == 8);

//------------------------------------------------------------------------
// std::vector<char>
//------------------------------------------------------------------------

constexpr char T(const std::vector<char>::const_iterator begin,
                 const std::vector<char>::const_iterator end)
{
    std::size_t size = end - begin;
    switch (size)
    {
        case 1:
            return *begin;
            break;
        case 2:
            return t(*begin, *(begin + 1));
            break;
        default:
            return t(T(begin, begin + size / 2), T(begin + size / 2, end));
            break;
    }
}

//------------------------------------------------------------------------

constexpr std::size_t HIFF(const std::vector<char>::const_iterator begin,
                           const std::vector<char>::const_iterator end)
{
    std::size_t size = end - begin;
    if (size == 1)
        return 1;
    else
        return size * f(T(begin, end)) + HIFF(begin, begin + size / 2) +
               HIFF(begin + size / 2, end);
}

//------------------------------------------------------------------------
// std::vector<bool>
//------------------------------------------------------------------------

constexpr char T(const std::vector<bool>::const_iterator begin,
                 const std::vector<bool>::const_iterator end)
{
    std::size_t size = end - begin;
    switch (size)
    {
        case 1:
            return '0' + *begin;
            break;
        case 2:
            return t('0' + *begin, '0' + *(begin + 1));
            break;
        default:
            return t(T(begin, begin + size / 2), T(begin + size / 2, end));
            break;
    }
}

//------------------------------------------------------------------------

constexpr std::size_t HIFF(const std::vector<bool>::const_iterator begin,
                           const std::vector<bool>::const_iterator end)
{
    std::size_t size = end - begin;
    if (size == 1)
        return 1;
    else
        return size * f(T(begin, end)) + HIFF(begin, begin + size / 2) +
               HIFF(begin + size / 2, end);
}

//------------------------------------------------------------------------

#endif // hiff_h
