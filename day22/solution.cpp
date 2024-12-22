#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    uint64_t result = 0;
    for (auto [secret]: input)
    {
        for (auto i: aoc::range(2000))
        {
            auto next = secret * 64;
            secret = (secret ^ next) % 16777216;
            next = secret / 32;
            secret = (secret ^ next) % 16777216;
            next = secret * 2048;
            secret = (secret ^ next) % 16777216;
        }

        result += secret;
    }

    return result;
}


template <typename T>
auto part2(T& input)
{
    // It is convenient to represent the sequence of 4 changes as a single number. 
    // Add 9 to the change to get a value in the range 0..18, which is base-19. 
    // Store the sequence as a 4-digit base-19 number.
    constexpr int kSequenceCap = 19 * 19 * 19 * 19;

    aoc::timer timer;

    // Use an array of int to mark the sequences already seen for current buyer.
    // Was previously an array of bool inside the loop, but this was slower due 
    // to numerous zero-initialisations. 
    array<int, kSequenceCap> sequences{};
    //map<uint32_t, int> total_prices{};
    //unordered_map<uint32_t, int> total_prices{};
    array<int, kSequenceCap> total_prices{};
    
    for (auto [secret]: input)
    {
        int      buyer    = secret;
        int      price    = secret % 10;
        int      sequence = 0;

        for (auto i: aoc::range(2000))
        {
            constexpr uint32_t kHashMax = 16777216;        // 1 << 24; 
            secret = (secret ^ (secret << 6)) % kHashMax;  // * 64
            secret = (secret ^ (secret >> 5)) % kHashMax;  // / 32
            secret = (secret ^ (secret << 11)) % kHashMax; // * 2048

            int price2 = secret % 10;
            int change = price2 - price;
            price = price2;

            // Store the sequence as a 4-digit base-19 number.
            sequence = (sequence * 19 + change + 9) % kSequenceCap;

            if (sequences[sequence] != buyer)
            {
                sequences[sequence] = buyer;                            
                total_prices[sequence] += price;
            }
        }
    }

    aoc::timer timer2;
    //return max_element(total_prices.begin(), total_prices.end(), 
    //    [](const auto&p1, const auto&p2){ return p1.second < p2.second; })->second;
    return *max_element(total_prices.begin(), total_prices.end());
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<uint32_t>(filename, R"((\d+))");

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
