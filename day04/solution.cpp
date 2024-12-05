#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    auto width  = input[0].size();
    auto height = input.size();

    auto horz = [&](auto i, auto j)
    {
        auto c0 = input[i][j];
        auto c1 = input[i+1][j];
        auto c2 = input[i+2][j];
        auto c3 = input[i+3][j];
        if ((c0 == 'X') && (c1 == 'M') && (c2 == 'A') && (c3 == 'S')) return true;
        if ((c3 == 'X') && (c2 == 'M') && (c1 == 'A') && (c0 == 'S')) return true;
        return false;
    };

    auto vert = [&](auto i, auto j)
    {
        auto c0 = input[i][j];
        auto c1 = input[i][j+1];
        auto c2 = input[i][j+2];
        auto c3 = input[i][j+3];
        if ((c0 == 'X') && (c1 == 'M') && (c2 == 'A') && (c3 == 'S')) return true;
        if ((c3 == 'X') && (c2 == 'M') && (c1 == 'A') && (c0 == 'S')) return true;
        return false;
    };

    auto diag = [&](auto i, auto j)
    {
        auto c0 = input[i][j];
        auto c1 = input[i+1][j+1];
        auto c2 = input[i+2][j+2];
        auto c3 = input[i+3][j+3];
        if ((c0 == 'X') && (c1 == 'M') && (c2 == 'A') && (c3 == 'S')) return true;
        if ((c3 == 'X') && (c2 == 'M') && (c1 == 'A') && (c0 == 'S')) return true;
        return false;
    };

    auto diag2 = [&](auto i, auto j)
    {
        auto c0 = input[i+3][j];
        auto c1 = input[i+2][j+1];
        auto c2 = input[i+1][j+2];
        auto c3 = input[i  ][j+3];
        if ((c0 == 'X') && (c1 == 'M') && (c2 == 'A') && (c3 == 'S')) return true;
        if ((c3 == 'X') && (c2 == 'M') && (c1 == 'A') && (c0 == 'S')) return true;
        return false;
    };

    int count{};
    for (auto x: aoc::range(width))
        for (auto y: aoc::range(height - 3))
            count += vert(x, y);

    for (auto x: aoc::range(width - 3))
        for (auto y: aoc::range(height))
            count += horz(x, y);

    for (auto x: aoc::range(width - 3))
        for (auto y: aoc::range(height - 3))
            count += diag(x, y) + diag2(x, y);

    return count;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    auto width  = input[0].size();
    auto height = input.size();

    auto diag = [&](auto i, auto j)
    {
        auto c0 = input[i][j];
        auto c1 = input[i+1][j+1];
        auto c2 = input[i+2][j+2];
        if ((c0 == 'M') && (c1 == 'A') && (c2 == 'S')) return true;
        if ((c2 == 'M') && (c1 == 'A') && (c0 == 'S')) return true;
        return false;
    };

    auto diag2 = [&](auto i, auto j)
    {
        auto c0 = input[i+2][j];
        auto c1 = input[i+1][j+1];
        auto c2 = input[i  ][j+2];
        if ((c0 == 'M') && (c1 == 'A') && (c2 == 'S')) return true;
        if ((c2 == 'M') && (c1 == 'A') && (c0 == 'S')) return true;
        return false;
    };

    int count{};
    for (auto x: aoc::range(width - 2))
        for (auto y: aoc::range(height - 2))
            count += diag(x, y) && diag2(x, y);

    return count;
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
