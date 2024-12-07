#include "utils.h"


struct Equation
{
    uint64_t result;
    vector<uint64_t> operands;
};


// Recursive binary search.
bool could_be_true(const Equation& e, uint64_t result, int index)
{
    // Condition increases part1 execution time.
    //if (result > e.result) return false;
    
    if (index == e.operands.size())
        return e.result == result;
    
    // Return early rather than evaluate further. Cheaper operator first.
    if (could_be_true(e, result + e.operands[index], index + 1)) return true;
    return could_be_true(e, result * e.operands[index], index + 1);
}


// Recursive trinary search.
bool could_be_true2(const Equation& e, uint64_t result, int index)
{
    // Condition reduces part2 execution time.
    if (result > e.result) return false;

    if (index == e.operands.size())
        return e.result == result;
    
    // Return early rather than evaluate further. Cheaper operator first.
    if (could_be_true2(e, result + e.operands[index], index + 1)) return true;
    if (could_be_true2(e, result * e.operands[index], index + 1)) return true;

    // Concatentation operator.
    auto temp = e.operands[index];
    while (temp > 0)
    {
        result *= 10;
        temp   /= 10;
    }
    return could_be_true2(e, result + e.operands[index], index + 1);
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    uint64_t total{};
    for (auto& e: input)
    {
        if (could_be_true(e, e.operands[0], 1)) total += e.result;
    }

    return total;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    uint64_t total{};
    for (auto& e: input)
    {
        if (could_be_true2(e, e.operands[0], 1)) total += e.result;
    }

    return total;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 

    vector<Equation> equations;
    for (auto& line: lines)
    {
        auto s = aoc::split(line, ":");
        Equation e;
        e.result = stoll(s[0]);
        e.operands = aoc::make_vector<uint64_t>(s[1], " ");
        equations.push_back(e);
    }

    auto p1 = part1(equations);
    cout << "Part1: " << p1 << '\n';
    
    auto p2 = part2(equations);
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
