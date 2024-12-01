#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <map>
#include <set>
#include <list>
#include <array>
#include <queue>
#include <stack>
#include <numeric>
#include <algorithm>
#include <limits>
#include <ranges>

#include <string>
#include <regex>
#include <filesystem>
#include <type_traits>
#include <chrono>
#include <tuple>
#include <cmath>

#include "conway.h"
#include "range.h"


using namespace std;
namespace fs = std::filesystem;
namespace cr = std::chrono;
namespace r  = std::ranges;
namespace v  = std::views;


namespace aoc {


// Trim leading and/or trailing whitespace from a string.
void ltrim(std::string& s);
void rtrim(std::string& s);
void trim(std::string& s);

// Replace all substrings matching "search" with "replace".
std::string replace(std::string source, const std::string& search, const std::string& replace);

enum class Blanks { Allow, Suppress };
enum class Trim   { Yes, No };

// Split a delimited string of tokens into a vector of string tokens. Trims substrings by default and drops trimmed 
// tokens which are empty by default. Not convinced how useful the option are, but you never know.
std::vector<std::string> split(std::string source, std::string delim, Blanks allow_blanks = Blanks::Suppress, Trim trim_subs = Trim::Yes);

// Read all lines from a file into a vector of strings. Trims lines by default and drops empty lines by default. Keeping 
// empty lines can be useful when the blank lines in the input are meaningful.
std::vector<std::string> read_lines(std::istream& is, Blanks allow_blanks = Blanks::Suppress, Trim trim_lines = Trim::Yes);
std::vector<std::string> read_lines(std::string filename, Blanks allow_blanks = Blanks::Suppress, Trim trim_lines = Trim::Yes);

// Calls read_lines() with Trim::Yes and Blanks::Allow, then partitions the results into a vector of strings for each group. 
std::vector<std::vector<std::string>> read_groups(std::istream& is);
std::vector<std::vector<std::string>> read_groups(std::string filename);

// Convert a list of delimited values into an array. Assumes all values are of the same type.
// "1,2,3" => vector<int>{1, 2, 3}.
template <typename T>
std::vector<T> make_vector(std::string input, std::string delim = ",")
{
    trim(input);
    input = replace(input, delim, " ");

    std::vector<T> result;
    std::istringstream is{input};
    while (!is.eof())
    {
        T value;
        is >> value;
        result.emplace_back(value);
    }

    return result;
}


// Iterate over a container of some tuple (e.g. created with read_lines()) and 
// return a vector of the I'th elements of each tuple instance.
template <size_t I, typename Tuple>
auto collect(const vector<Tuple>& input) 
{
    using T = tuple_element_t<I, Tuple>;
    vector <T> result;
    result.reserve(input.size());
    for (const auto& t: input)
    {
        result.emplace_back(get<I>(t));
    }
    return result;
}


// Extract arguments from a formatted string into a tuple, via regex.
// .      not newline any character except line terminators (LF, CR, LS, PS).
// \t     tab (HT)
// \n     newline (LF)
// \v     vertical tab (VT)
// \f     form feed (FF)
// \r     carriage return (CR)
// \xhh	  ASCII character
// \uhhhh unicode character
// \d     digit
// \D     not digit
// \s     whitespace
// \S     not whitespace
// \w     word: an alphanumeric or underscore character (same as [_[:alnum:]]).
// \W     not word
template <class... Args>
std::tuple<Args...> parse_line(const std::regex& re, const std::string& str, bool& matched, const std::string& delim = ";")
{
    // Capture values from a string using regex, and stuff these
    // into a new space-delimited string.
    std::smatch matches;
    std::stringstream ss{};

    matched = std::regex_match(str, matches, re);
    if (matched)
    {
        for (int i = 1; i < matches.size(); ++i)
        {
            // Replace spaces or the read back from ss below doesn't work properly.
            // This caused a world of pain on Day 16 2022 (as if the problem wasn't 
            // hard enough already). The delimiter can be used later to split the string.
            string temp = matches[i];
            temp = replace(temp, " ", delim);
            ss << temp << " ";
        }
    }

    // Read from the space-delimited string into a tuple.
    auto res = std::tuple<Args...>{};
    std::apply
    (
        [&ss](Args&... args)
        {
            ((ss >> args), ...);
        }, res
    );
    return res;
}


template <class... Args>
std::tuple<Args...> parse_line(const std::string& pat, const std::string& str, bool& matched, const std::string& delim = ";")
{
    const std::regex re(pat);
    return parse_line<Args...>(re, str, matched, delim);
}


template <class... Args>
std::tuple<Args...> parse_line(const std::string& pat, const std::string& str, const std::string& delim = ";")
{
    bool dummy;
    const std::regex re(pat);
    return parse_line<Args...>(re, str, dummy, delim);
}


// Read all lines from a file and parse with regex to create array of tuples.
template <class... Args>
std::vector<std::tuple<Args...>> read_lines(std::istream& is, std::string pat, const std::string& delim = ";")
{
    std::vector<std::tuple<Args...>> result;
    const std::regex re(pat);

    auto lines = read_lines(is, Blanks::Suppress);
    for (const auto& line: lines)
    {
        // Skip lines which don't match - useful for reading files with two or 
        // more lines formats. Call read_lines for each format.
        bool matched = false;
        auto temp    = parse_line<Args...>(re, line, matched, delim);
        if (matched)
            result.push_back(temp);
    }

    return result;
}


template <class... Args>
std::vector<std::tuple<Args...>> read_lines(std::string filename, std::string pat, const std::string& delim = ";")
{
    std::ifstream is{filename};
    return read_lines<Args...>(is, pat, delim);
}


template <typename T, typename U>
void check_result(T value, U expected)
{
    static_assert(std::is_integral_v<T> && std::is_integral_v<U>);
    static_assert(sizeof(T) >= sizeof(U));
    static_assert(std::is_signed_v<T> == std::is_signed_v<U>);

    if (value != static_cast<T>(expected))
    {
        std::cout << "ERROR: expected = " << expected << " but got = " << value << '\n';
    }
}


// Used for a default predicate below
template <typename T>
struct AlwaysTrue
{
    bool operator()(T) const { return true; }
};


// Used for a default transform below
template <typename T>
struct NullTransform
{
    T operator()(T t) const { return t; }
};


// Simple equivalent to Python: [trans(x) for x in src if pred(x)].
//    std::string s = "a234234dfadfkjasdfkjadfjl;akdf";
//    // Effectively [x for x in s].
//    auto x = comprehend(s);  
//    // Effectively [trans(x) for x in s].
//    auto x = comprehend(s, [](auto c) -> std::size_t { return c; });  
//    // Effectively [trans(x) for x in s if pred(x)].
//    auto x = comprehend(s, [](auto c) -> std::size_t { return c; }, [](auto c) { return c % 10 == 7; });  
template <typename Src, 
    typename Trans = NullTransform<typename Src::value_type>, 
    typename Pred = AlwaysTrue<typename Src::value_type>>
auto comprehend(const Src src, 
    const Trans trans = NullTransform<typename Src::value_type>{}, 
    const Pred pred = AlwaysTrue<typename Src::value_type>{})
{
    std::vector<decltype(trans(src[0]))> dest;
    for (const auto& value: src)
    {
        if (pred(value))
            dest.emplace_back(trans(value));
    }
    return dest;
}


vector<string> make_grid_with_border(const vector<string>& input, size_t width, char value);
vector<string> read_grid_with_border(std::string filename, size_t width, char value);


template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}


class timer
{
public:
    timer(std::string label = "")
    : m_start{cr::steady_clock::now()}
    , m_label{label}
    {
    }
    
    ~timer()
    {
        auto end = cr::steady_clock::now();
        cr::duration<double> diff = end - m_start;
        if (m_label.size() > 0)
            //std::cout << "Time elapsed: " << m_label << ": " << (diff.count() * 1000) << "ms\n";
            std::cout << "Time elapsed: " << m_label << ": " << (diff.count() * 1000000) << "us\n";
        else 
            //std::cout << "Time elapsed: " << (diff.count() * 1000) << "ms\n";
            std::cout << "Time elapsed: " << (diff.count() * 1000000) << "us\n";
    }

private:
    cr::time_point<cr::steady_clock> m_start{}; 
    std::string m_label{};
};


template <typename T>
T gcd(T a, T b)
{
    while (b != 0)
    {
        T t = b;
        b = a % b;
        a = t;
    }
    return a;
}


template <typename T>
T lcm(T a, T b)
{
    return abs(a) * (abs(b) / gcd(a, b));
}


} // namespace aoc {


