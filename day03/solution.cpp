#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int total{};
    for (auto mem: input)
    {
        regex  r(R"(mul\(([+-]?\d+),([+-]?\d+)\))", regex_constants::icase);
        smatch m;
        
        string::const_iterator start(mem.cbegin());
        while (regex_search(start, mem.cend(), m, r))
        {
            total += stoi(m.str(1)) * stoi(m.str(2));
            start = m.suffix().first;
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
        regex  r_mul(R"(mul\(([+-]?\d+),([+-]?\d+)\))", regex_constants::icase);
        smatch m_mul;

        regex  r_do(R"(do\(\))", regex_constants::icase);
        smatch m_do;

        regex  r_dont(R"(don't\(\))", regex_constants::icase);
        smatch m_dont;

        string::const_iterator start(mem.cbegin());
        while (regex_search(start, mem.cend(), m_mul, r_mul))
        {
            // Also scan for the next do() or don't().
            regex_search(start, mem.cend(), m_do, r_do);
            regex_search(start, mem.cend(), m_dont, r_dont);

            // Use the suffix() iterators to determine which of the three instructions comes next.     
            auto i_mul  = m_mul.suffix().first;
            auto i_do   = m_do.suffix().first;
            auto i_dont = m_dont.suffix().first;
            start = min(i_mul, min(i_do, i_dont));

            // Perform the instruction.
            if (start == i_mul)
            {
                total += stoi(m_mul.str(1)) * stoi(m_mul.str(2)) * enable;
            }
            else if (start == i_do)
            {
                enable = 1;                
            }
            else if (start == i_dont)
            {
                enable = 0;
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
