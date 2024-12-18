#include "utils.h"


constexpr int Unvisited = 0x7FFF'FFFF;
constexpr int Corrupt   = -1;

// For the example
// constexpr size_t Rows  = 7;
// constexpr size_t Cols  = 7;
// constexpr size_t Steps = 12;

// For the problem 
constexpr size_t Rows  = 71;
constexpr size_t Cols  = 71;
constexpr size_t Steps = 1024;


template<typename T>
vector<vector<T>> make_grid(size_t rows, size_t cols, T value)
{
    vector<T> row(cols, value);
    vector<vector<T>> grid(rows, row);
    return grid;
}


template<typename T>
vector<vector<T>> extend_grid(const vector<vector<T>>& grid, size_t width, T value)
{
    const auto rows = grid.size();
    const auto cols = grid[0].size();   
    auto border_grid = make_grid<T>(rows + 2, cols + 2, value);
    for (auto r: aoc::range(rows))
        for (auto c: aoc::range(cols))
            border_grid[r + 1][c + 1] = grid[r][c];
    return border_grid;
}


template <typename T>
int run(const T& input, int steps)
{
    auto temp = make_grid(Rows, Cols, Unvisited);
    auto grid = extend_grid(temp, 1, Corrupt);
    for (auto s: aoc::range(steps))
    {
        auto [c, r] = input[s];
        grid[r + 1][c + 1] = Corrupt;
    }

    set<pair<int, int>> visited;
    set<pair<int, int>> front;
    front.insert({1, 1});
    grid[1][1] = 0;

    while (front.size() > 0)
    {        
        set<pair<int, int>> front2;
        for (auto [r, c]: front)
        {
            visited.insert({r, c});
            if (grid[r][c] == Corrupt) continue;

            auto steps = grid[r][c] + 1;
            if (grid[r + 1][c] != Corrupt) grid[r + 1][c] = min(steps, grid[r + 1][c]);
            if (grid[r - 1][c] != Corrupt) grid[r - 1][c] = min(steps, grid[r - 1][c]);
            if (grid[r][c + 1] != Corrupt) grid[r][c + 1] = min(steps, grid[r][c + 1]);
            if (grid[r][c - 1] != Corrupt) grid[r][c - 1] = min(steps, grid[r][c - 1]);
            
            if (visited.find({r + 1, c}) == visited.end()) front2.insert({r + 1, c});
            if (visited.find({r - 1, c}) == visited.end()) front2.insert({r - 1, c});
            if (visited.find({r, c + 1}) == visited.end()) front2.insert({r, c + 1});
            if (visited.find({r, c - 1}) == visited.end()) front2.insert({r, c - 1});
        }

        front = front2;
    }

    return grid[Rows][Cols];
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;
    return run(input, Steps);
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    // Surely there is a better way than repeating the simulation.
    // At least a binary chop would reduce the number of runs.
    for (auto steps: aoc::range(Steps, input.size()))
    {
        if (run(input, steps) == Unvisited) 
        {
            auto [c, r] = input[steps - 1];
            cout << c << "," << r << endl;
            break;
        }
    }

    return 0;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<int, int>(filename, R"((\d+),(\d+))");

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
