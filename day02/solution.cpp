#include "utils.h"


bool is_valid_report(const auto& r)
{
    bool valid = true;
    auto dir   = aoc::sgn(r[1] - r[0]);
    for (auto i: aoc::range(r.size() - 1))
    {
        auto del  = abs(r[i + 1] - r[i]);
        auto sgn  = aoc::sgn(r[i + 1] - r[i]);
        valid &= (del >= 1) && (del <= 3) && (sgn == dir);
    }
    return valid;
};


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int safe = 0;
    for (auto& r: input)
    {
        safe += is_valid_report(r);
    }

    return safe;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    int safe = 0;
    for (auto& r: input)
    {
        if (is_valid_report(r))
        {
            ++safe;
        }
        else
        {
            for (auto i: aoc::range(r.size()))
            {
                auto r2 = r;
                r2.erase(r2.begin() + i);
                if (is_valid_report(r2))
                {
                    ++safe;
                    break;
                }
            }
        }
    }

    return safe;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 
    vector<vector<int>> input;
    for (auto line: lines)
    {
        auto v = aoc::make_vector<int>(line, " ");
        input.push_back(v);
        // for (auto i: v) cout << i << " ";
        // cout << endl;
    }

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
