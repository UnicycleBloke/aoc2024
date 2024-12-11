#include "utils.h"


uint64_t count_stones(vector<uint64_t> input, int blinks)
{
    // This reminds of the Lantern Fish problem.
    //
    // Original Part 1 used a vector but this just won't work for Part 2.
    // Fretted about keeping the stones in order, but this is a red herring. 
    // We only care about how many stones we have with each number. 
    //    
    // The numbers engraved get huge, as do the counts of stones with a given number.
    // This seems to rule out an array or something hold the results. We end up with
    // thousands of distinct numbers, scattered across the uint64_t range. Use a map. 
    unordered_map<uint64_t, uint64_t> stones;
    for (auto s: input)
    {
        stones[s] = stones[s] + 1;
    }

    auto digits = [](uint64_t s)
    {
        int d = 0;
        while (s > 0)
        {
            ++d;
            s = s / 10; 
        }
        return d;
    };

    auto pow10 = [](int d)
    {
        uint64_t result = 1;
        for (auto i: aoc::range(d)) result = result * 10;
        return result;
    };

    for (auto blink: aoc::range(blinks))
    {
        // Implement the recurrence relation.
        unordered_map<uint64_t, uint64_t> stones2;
        for (auto [s, count]: stones)
        {
            if (s == 0)
            {
                stones2[1] = stones2[1] + count;
            }
            else if (int d = digits(s); (d % 2) == 0)
            {
                auto m   = pow10(d/2);
                auto div = s / m;
                auto mod = s % m;
                stones2[div] = stones2[div] + count;
                stones2[mod] = stones2[mod] + count;
            }
            else
            {
                auto s2 = s * 2024;
                stones2[s2] = stones2[s2] + count;
            } 
        }

        stones = stones2;
    }

    uint64_t total{};
    for (auto [s, count]: stones)
    {
        total += count;
    }

    return total;
}


template <typename T>
auto part1(T& input)
{
    aoc::timer timer;
    return count_stones(input, 25);
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;
    return count_stones(input, 75);
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 
    auto input = aoc::make_vector<uint64_t>(lines[0], " ");

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';   

    auto p2 = part2(input);
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
