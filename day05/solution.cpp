#include "utils.h"


template <typename T>
bool is_valid(const T& rules, const vector<int>& update)
{
    for (const auto& [a, b]: rules)
    {
        // We only care if both numbers are present in the update
        auto ita = r::find(update, a);
        if (ita == update.end()) continue;
        auto itb = r::find(update, b);
        if (itb == update.end()) continue;

        if (ita >= itb) return false;
    }
    return true;
}


template <typename T>
auto part1(const T& rules, const vector<vector<int>>& updates)
{
    aoc::timer timer;

    int total = 0;
    for (const auto& u: updates)
        total += is_valid(rules, u) ? u[u.size() / 2] : 0;

    return total;
}


template <typename T>
auto part2(const T& rules, const vector<vector<int>>& updates)
{
    aoc::timer timer;

    int total = 0;
    for (auto u: updates)
    {
        // Already valid - skip
        if (is_valid(rules, u)) continue;

        // Loop until u becomes valid
        while (true)
        {
            bool valid = true;
            for (const auto& [a, b]: rules)
            {
                auto ita = r::find(u, a);
                if (ita == u.end()) continue;

                auto itb = r::find(u, b);
                if (itb == u.end()) continue;

                // Swap pairs which break a rule
                // We effectively sort until all rules are satisfied
                if (ita >= itb)
                {
                    auto temp = *ita;
                    *ita = *itb;
                    *itb = temp;
                    valid = false;
                    break;
                }
            }

            if (valid) break;
        }
        
        total += u[u.size() / 2];
    }

    return total;
}


void run(const char* filename)
{
    auto rules = aoc::read_lines<int,int>(filename, R"((\d+)\|(\d+))");

    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 
    vector<vector<int>> updates;
    for (auto line: lines)
    {
        if (line[2] == '|') continue;
        updates.push_back(aoc::make_vector<int>(line, ","));
    }   

    auto p1 = part1(rules, updates);
    cout << "Part1: " << p1 << '\n';
    
    auto p2 = part2(rules, updates);
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
