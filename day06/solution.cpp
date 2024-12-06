#include "utils.h"


// More efficient than creating copies of the original input.
template <typename T>
void reuse_grid(T& grid, int xstart, int ystart)
{
    const int xmax = grid[0].size();
    const int ymax = grid.size();

    for (auto i: aoc::range(xmax))
        for (auto j: aoc::range(ymax))
            if (grid[j][i] != '#') grid[j][i] = '.';
    grid[ystart][xstart] = '^';
}


template <typename T>
auto patrol(T& grid, int xstart, int ystart, int& steps)
{
    const int xmax = grid[0].size();
    const int ymax = grid.size();

    int x  = xstart;
    int y  = ystart;
    grid[y][x] = '.';

    steps = 0;

    char d = '^';
    while (true)
    {
        // Have we been here before, facing in this direction? In a loop.
        if (grid[y][x] == d) return true;

        // Mark our visit to this location
        if (grid[y][x] == '.') 
        {
            grid[y][x] = d;
            ++steps;
        }

        // Select the next location
        int x2 = x;
        int y2 = y;
        switch (d)
        {
            case '^': --y2; break;
            case '>': ++x2; break;
            case 'v': ++y2; break;
            case '<': --x2; break;
        }

        // Have we gone over the edge? Not a loop
        if ((x2 < 0) || (x2 >= xmax) || (y2 < 0) || (y2 >= ymax)) return false;

        // Do we need to turn right?
        if ((grid[y2][x2] == '#') || (grid[y2][x2] == '@'))
        {
            switch (d)
            {
                case '^': d = '>'; break;
                case '>': d = 'v'; break;
                case 'v': d = '<'; break;
                case '<': d = '^'; break;
            }
        }
        else
        {
            x = x2;
            y = y2;
        }
    }
    
    return false;
}


template <typename T>
auto part1(T grid, int xstart, int ystart)
{
    aoc::timer timer;

    int steps = 0;
    patrol(grid, xstart, ystart, steps);
    
    return steps;
}


template <typename T>
auto part2(T& grid, int xstart, int ystart)
{
    aoc::timer timer;

    const int xmax = grid[0].size();
    const int ymax = grid.size();

    // We only need to test locations on the unobstructed route.
    T   route = grid;
    int dummy = 0;
    patrol(route, xstart, ystart, dummy);

    int loops = 0;
    for (auto i: aoc::range(xmax))
    {
        for (auto j: aoc::range(ymax))
        {            
            if ((i == xstart) && (j == ystart)) continue;

            if (route[j][i] == '#') continue;
            if (route[j][i] == '.') continue;

            reuse_grid(grid, xstart, ystart); 
            grid[j][i] = '@';

            int steps = 0;
            loops += patrol(grid, xstart, ystart, steps);
        }
    }

    return loops;
}


void run(const char* filename)
{
    auto grid   = aoc::read_lines(filename, aoc::Blanks::Suppress); 
    
    int xmax = grid[0].size();
    int ymax = grid.size();

    int  xstart = 0;
    int  ystart = 0;
    for (auto i: aoc::range(xmax))
    {
        for (auto j: aoc::range(ymax))
        {
            if (grid[j][i] == '^')
            {
                xstart = i; 
                ystart = j; 
                break;
            }
        }
    }

    auto p1 = part1(grid, xstart, ystart);
    cout << "Part1: " << p1 << '\n';
   
    auto p2 = part2(grid, xstart, ystart);
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
