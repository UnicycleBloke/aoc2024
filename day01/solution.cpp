#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    auto va = aoc::collect<0>(input);
    auto vb = aoc::collect<1>(input);

    r::sort(va);
    r::sort(vb);

    int result = 0;
    for (auto i: aoc::range(va.size()))
    {
        result += abs(va[i] - vb[i]);
    }

    return result;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    auto va = aoc::collect<0>(input);
    auto vb = aoc::collect<1>(input);

    int result = 0;
    for (auto i: aoc::range(va.size()))
    {
        result += va[i] * r::count(vb, va[i]);
    }

    return result;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<int,int>(filename, R"((\d+)\s+(\d+))");

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
