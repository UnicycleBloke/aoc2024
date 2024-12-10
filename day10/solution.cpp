#include "utils.h"


template <typename T>
auto count_trails(const T& input, int r, int c)
{
    const int Rows = input.size();
    const int Cols = input[0].size();

    auto in_bounds = [&](int row, int col)
    {
        return(row >= 0) && (row < Rows) && (col >= 0) && (col < Cols);
    };

    int heads = 0;

    // Use a set to avoid double duplicating locations on multiple trails.
    using Head = pair<int, int>;
    set<Head> trails;
    trails.insert(Head{r, c});

    while (true)
    {        
        set<Head> trails2;
        for (auto [r1, c1]: trails)
        {
            if (input[r1][c1] == '9') 
            {
                ++heads;
                continue;
            }

            int r2 = r1 + 1;
            int c2 = c1;
            if (in_bounds(r2, c2) && (input[r2][c2] - input[r1][c1] == 1))
                trails2.insert(Head{r2, c2});

            r2 = r1 - 1;
            c2 = c1;
            if (in_bounds(r2, c2) && (input[r2][c2] - input[r1][c1] == 1))
                trails2.insert(Head{r2, c2});

            r2 = r1;
            c2 = c1 + 1;
            if (in_bounds(r2, c2) && (input[r2][c2] - input[r1][c1] == 1))
                trails2.insert(Head{r2, c2});

            r2 = r1;
            c2 = c1 - 1;
            if (in_bounds(r2, c2) && (input[r2][c2] - input[r1][c1] == 1))
                trails2.insert(Head{r2, c2});
        }

        if (trails2.size() == 0) break;
        trails = trails2;
    }

    return heads;    
}


template <typename T>
auto count_trails2(const T& input, int r, int c)
{
    const int Rows = input.size();
    const int Cols = input[0].size();

    auto in_bounds = [&](int row, int col)
    {
        return(row >= 0) && (row < Rows) && (col >= 0) && (col < Cols);
    };

    int heads = 0;

    // Use a vector to duplicate locations on multiple trails.
    using Head = pair<int, int>;
    vector<Head> trails;
    trails.push_back(Head{r, c});

    while (true)
    {        
        vector<Head> trails2;
        for (auto [r1, c1]: trails)
        {
            if (input[r1][c1] == '9') 
            {
                ++heads;
                continue;
            }

            int r2 = r1 + 1;
            int c2 = c1;
            if (in_bounds(r2, c2) && (input[r2][c2] - input[r1][c1] == 1))
                trails2.push_back(Head{r2, c2});

            r2 = r1 - 1;
            c2 = c1;
            if (in_bounds(r2, c2) && (input[r2][c2] - input[r1][c1] == 1))
                trails2.push_back(Head{r2, c2});

            r2 = r1;
            c2 = c1 + 1;
            if (in_bounds(r2, c2) && (input[r2][c2] - input[r1][c1] == 1))
                trails2.push_back(Head{r2, c2});

            r2 = r1;
            c2 = c1 - 1;
            if (in_bounds(r2, c2) && (input[r2][c2] - input[r1][c1] == 1))
                trails2.push_back(Head{r2, c2});
        }

        if (trails2.size() == 0) break;
        trails = trails2;
    }

    return heads;    
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int trails = 0;
    for (auto r: aoc::range(input.size()))
    {
        for (auto c: aoc::range(input[0].size()))
        {
            if (input[r][c] == '0')
                trails += count_trails(input, r, c);        
        }
    }

    return trails;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    int trails = 0;
    for (auto r: aoc::range(input.size()))
    {
        for (auto c: aoc::range(input[0].size()))
        {
            if (input[r][c] == '0')
                trails += count_trails2(input, r, c);        
        }
    }

    return trails;
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
