#include "utils.h"


struct Equation
{
    uint64_t result{};
    vector<uint64_t> operands{};

    Equation() = default;
    Equation(const Equation&) = delete;
    Equation& operator=(const Equation&) = delete;
    Equation(Equation&&) = default;
    Equation& operator=(Equation&&) = default;
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



// Recursive trinary search.
bool could_be_true3(uint64_t target, const uint64_t* operands, int numops, uint64_t result, int index)
{
    // Condition reduces part2 execution time.
    if (result > target) return false;

    if (index == numops)
        return target == result;
    
    // Return early rather than evaluate further. Cheaper operator first.
    if (could_be_true3(target, operands, numops, result + operands[index], index + 1)) return true;
    if (could_be_true3(target, operands, numops, result * operands[index], index + 1)) return true;

    // Concatentation operator.
    auto temp = operands[index];
    while (temp > 0)
    {
        result *= 10;
        temp   /= 10;
    }
    return could_be_true3(target, operands, numops, result + operands[index], index + 1);
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
auto part2(T& input)
{
    aoc::timer timer;

    uint64_t total{};
    for (const auto& e: input)
    {
        if (could_be_true2(e, e.operands[0], 1)) total += e.result;
    }

    return total;
}

template <typename T>
auto part3(T& input)
{
    aoc::timer timer;

    uint64_t total{};
    for (const auto& e: input)
    {
        if (could_be_true3(e.result, &e.operands[0], e.operands.size(), e.operands[0], 1)) total += e.result;
    }

    return total;
}


#include <span>

// bool is_valid(uint64_t result, const span<const uint64_t>& operands)     
// {
//     auto l = operands.size();
//     if (l == 1) 
//     {
//         return result == operands[0];
//     }
//     auto last = operands[l - 1];

//     if (result % last == 0) 
//     {
//         if (is_valid(result / last, span<const uint64_t>(operands.begin(), operands.end() - 1)))
//         {
//             return true;
//         }
//     }

//     auto sub = result - last;
//     if (sub >= 0) 
//     {
//         return is_valid(sub, span<const uint64_t>(operands.begin(), operands.end() - 1));
//     }

//     return false;
// }


// bool is_valid2(uint64_t result, span<const uint32_t> operands)     
// {
//     auto l = operands.size();
//     if (l == 1) 
//         return result == operands[0];

//     uint32_t last = operands[l - 1];
//     if (result % last == 0) 
//         if (is_valid2(result / last, span<const uint32_t>(operands.begin(), operands.end() - 1)))
//             return true;

//     auto sub = result - last;
//     if (sub >= 0) 
//     {
//         if (is_valid2(sub, span<const uint32_t>(operands.begin(), operands.end() - 1)))
//             return true;

//         auto p = 10;       
//         uint32_t temp = last;
//         while (temp >= 10)
//         {
//             p    *= 10;
//             temp /= 10;
//         }
//         if (sub % p == 0) 
//         {
//             if (is_valid2(sub / p, span<const uint32_t>(operands.begin(), operands.end() - 1)))
//                 return true;
//         }
//     }

//     return false;
// }


// uint64_t part1_rust(const vector<Equation>& equations) 
// {
//     aoc::timer timer;

//     uint64_t total{};
//     for (const auto& [result, operands]: equations)
//     {
//         if (is_valid(result, span<const uint64_t>{operands})) total += result;
//     }
//     return total;
// }


// uint64_t part2_rust(const vector<Equation>& equations) 
// {
//     aoc::timer timer;

//     uint64_t total{};
//     for (const auto& [result, operands]: equations)
//     {
//         if (is_valid2(result, span<const uint32_t>{operands})) total += result;
//     }
//     return total;
// }


// template <typename T>
// auto part1_matt(const T& input)
// {
//     aoc::timer timer;

//     uint64_t result{};

//     for (const auto& e: input)
//     {
//         int      length = e.operands.size();
//         int      count  = 1 << length; 

//         for (int p = 0; p < count; ++p)
//         {
//             uint64_t output = e.operands[0];
            
//             for (int i = 1; i < length; ++i)
//             {
//                 int q = 1 << (i - 1);
//                 output = (p & q) == 0 ? output * e.operands[i] : output + e.operands[i];
//                 if (output > e.result) break;
//             }

//             if (output == e.result)
//             {
//                 result += output;
//                 break;
//             }
//         }
//     }

//     return result;
// }


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 

    vector<Equation> equations;
    for (const auto& line: lines)
    {
        auto s = aoc::split(line, ":");
        Equation e;
        e.result = stoll(s[0]);
        e.operands = aoc::make_vector<uint64_t>(s[1], " ");
        equations.push_back(move(e));
    }

    auto p1 = part1(equations);
    cout << "Part1: " << p1 << '\n';

    //auto p1r = part1_rust(equations);
    //cout << "Part1 Rust: " << p1r << '\n';

    //auto p1m = part1_matt(equations);
    //cout << "Part1 Matt: " << p1m << '\n';

    auto p2 = part2(equations);
    cout << "Part2: " << p2 << '\n';

    auto p2b = part3(equations);
    cout << "Part2b: " << p2b << '\n';
   
    //auto p2r = part2(equations);
    //cout << "Part2 Rust: " << p2r << '\n';
}


int main(int argc, char** argv)
{
    aoc::timer timer;
    //try
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
    // catch (std::exception& e)
    // {
    //     cout << e.what();
    // }
}
