#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int total{};
    for (auto mem: input)
    {
        regex  r(R"(mul\(([+-]?\d+),([+-]?\d+)\))", regex_constants::icase);

        vector<smatch> matches{sregex_iterator{mem.cbegin(), mem.cend(), r}, sregex_iterator{}};
        for (auto m: matches)
        {
            total += stoi(m.str(1)) * stoi(m.str(2));
        } 
    }

    return total;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    // Initially enabled.
    int enable = 1;

    int total{};
    for (auto mem: input)
    {
        regex  r(R"(mul\(([+-]?\d+),([+-]?\d+)\)|do\(\)|don't\(\))", regex_constants::icase);

        vector<smatch> matches{sregex_iterator{mem.cbegin(), mem.cend(), r}, sregex_iterator{}};
        for (auto m: matches)
        {
            if (m.str() == "do()")
            {
                enable = 1;                
            }
            else if (m.str() == "don't()")
            {
                enable = 0;
            }
            else
            {
                total += stoi(m.str(1)) * stoi(m.str(2)) * enable;
            }
        }
    }

    return total;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    
    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
}


int main(int argc, char** argv)
{
    aoc::timer timer;
    try
    {
        if (argc < 2)
        {
            cout << "Provide input file name\n";
            return -1;
        }

        fs::path path{argv[1]};
        if (!fs::is_regular_file(path))
        {
            cout << "Path '" << argv[1] << "' does not exist or is not a file\n";
            return -1;
        }

        run(argv[1]);
    }
    catch (std::exception& e)
    {
        cout << e.what();
    }
}
