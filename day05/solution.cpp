#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;
    return 0;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;
    return 0;
}


// vector<tuple<Args...>>
//auto lines = aoc::read_lines<int,int,int,int>(filename, R"((\d+)-(\d+),(\d+)-(\d+))");

// vector<string>    
//auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 

// Replace all substrings matching "search" with "replace".
//std::string replace(std::string source, const std::string& search, const std::string& replace);

// Split a delimited string of tokens into a vector of string tokens. Trims substrings by default and drops trimmed 
// tokens which are empty by default. Not convinced how useful the option are, but you never know.
//std::vector<std::string> split(std::string source, std::string delim, Blanks allow_blanks = Blanks::Suppress, Trim trim_subs = Trim::Yes);

// vector (size_type n, const value_type& val = value_type(),
//vector<int> row(COLS, 0);
//vector<vector<int>> grid(ROWS, row);
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
