#include "utils.h"
#include <span>


struct Equation
{
    uint64_t result{};
    vector<uint32_t> operands{};

    Equation() = default;
    Equation(const Equation&) = delete;
    Equation& operator=(const Equation&) = delete;
    Equation(Equation&&) = default;
    Equation& operator=(Equation&&) = default;
};


int calls_cpp{};
int calls_rust{};

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
    ++calls_cpp;

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
    for (const auto& e: input)
    {
        if (could_be_true(e, e.operands[0], 1)) total += e.result;
    }

    return total;
}


template <typename T>
auto part2_cpp(const T& input)
{
    aoc::timer timer;

    uint64_t total{};
    for (const auto& e: input)
    {
        if (could_be_true2(e, e.operands[0], 1)) total += e.result;
    }

    return total;
}


// This was translated from a *much* faster Rust algorithm. I somehow mistimed it and 
// got a comparable duration to my algo. There followed much confusion and desparate
// attempts to work out why the C++ was an order of magnitude slower than Rust. They 
// should be comparable. Nothing obviously wrong in my code. It turns out (when someone
// tried it and didn't mistime it), that the Rust algo is just far better at pruning
// the search space. 
//
// I didn't commit the code which caused so much confusion but, strangely, the version
// of the translated algo which I shared on Godbolt (the function below) was absolutely
// fine. I'm baffled by how I mistimed it on my machine before but now it works as expected.
// More haste less speed. Too much belief in an erroneous result. Oh well. 
//
// I used span to simulate Rust slices, but this wasn't a factor. Or at least not a major one.
// Non-span version below.
// bool is_valid2(uint64_t result, span<const uint32_t> operands)     
// {
//     ++calls_rust;

//     auto l = operands.size();
//     if (l == 1) {
//         return result == operands[0];
//     }
//     uint32_t last = operands[l - 1];
//     if (result % last == 0) {
//         if (is_valid2(result / last, span<const uint32_t>(operands.begin(), operands.end() - 1))) {
//             return true;
//         }
//     }
//     auto sub = result - last;
//     if (sub >= 0) {
//         if (is_valid2(sub, span<const uint32_t>(operands.begin(), operands.end() - 1))) {
//             return true;
//         }

//         auto p = 10;       
//         // Inlined the Rust code for this bit
//         uint32_t temp = last;
//         while (temp >= 10) {
//             p    *= 10;
//             temp /= 10;
//         }
//         if (sub % p == 0) {
//             if (is_valid2(sub / p, span<const uint32_t>(operands.begin(), operands.end() - 1))) {
//                 return true;
//             }
//         }
//     }

//     return false;
// }


bool is_valid2(const vector<uint32_t>& operands, uint64_t result, int index)
{
    ++calls_rust;

    auto l = operands.size() - index;
    if (l == 1) {
        return result == operands[0];
    }
    uint32_t last = operands[l - 1];
    if (result % last == 0) {
        if (is_valid2(operands, result / last, index + 1)) {
            return true;
        }
    }
    auto sub = result - last;
    if (sub >= 0) {
        if (is_valid2(operands, sub, index + 1)) {
            return true;
        }

        auto p = 10;       
        // Inlined the Rust code for this bit
        uint32_t temp = last;
        while (temp >= 10) {
            p    *= 10;
            temp /= 10;
        }
        if (sub % p == 0) {
            if (is_valid2(operands, sub / p, index + 1)) {
                return true;
            }
        }
    }

    return false;
}


template <typename T>
auto part2_rust(const T& input)
{
    aoc::timer timer;

    // Not too familiar with ranges/views, so used a boring old loop.
    uint64_t total{};
    for (const auto& [result, operands]: input)
    {
        if (is_valid2(operands, result, 0)) total += result;
    }
    return total;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 

    vector<Equation> equations;
    for (const auto& line: lines)
    {
        auto s = aoc::split(line, ":");
        Equation e;
        e.result = stoll(s[0]);
        e.operands = aoc::make_vector<uint32_t>(s[1], " ");
        equations.push_back(move(e));
    }

    auto p1 = part1(equations);
    cout << "Part1: " << p1 << '\n';

    auto p2a = part2_cpp(equations);
    cout << "Part2 (My algo):   " << p2a << " calls=" << calls_cpp << '\n';

    auto p2b = part2_rust(equations);
    cout << "Part2 (Rust algo): " << p2b << " calls=" << calls_rust << '\n';
}


int main(int argc, char** argv)
{
    std::ios::sync_with_stdio(false);

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
