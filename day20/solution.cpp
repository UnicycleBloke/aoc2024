#include "utils.h"


int run(vector<string> grid, int tick_cheat, char dir_cheat)
{
    //aoc::timer timer;

    int Rows = grid.size();
    int Cols = grid[0].size();

    int rs, cs, re, ce;

    for (auto r: aoc::range(Rows))
    {
        for (auto c: aoc::range(Cols))
        {
            if (grid[r][c] == 'S') 
            {
                rs = r; cs = c;                
            }
            if (grid[r][c] == 'E') 
            {
                re = r; ce = c;                
            }
        }
    }

    set<pair<int, int>>      visited;
    set<pair<int, int>>      front;
    map<pair<int, int>, int> costs;

    front.insert({rs, cs});
    costs[{rs, cs}] = 0;

    int tick = -1;
    while (front.size() > 0)
    {        
        ++tick;

        set<pair<int, int>> front2;
        for (auto [r, c]: front)
        {
            if ((r == re) && (c == ce)) 
                return tick;

            visited.insert({r, c});

            if ((grid[r][c] == '#'))
                continue;

            if (tick == tick_cheat)
            {
                switch (dir_cheat)
                {
                    case 'N': if (r > 0)          grid[r - 1][c] = '.'; break;
                    case 'S': if (r < (Rows - 1)) grid[r + 1][c] = '.'; break;
                    case 'E': if (c < (Cols - 1)) grid[r][c + 1] = '.'; break;
                    case 'W': if (c > 0)          grid[r][c - 1] = '.'; break;
                }
            }


            // //bool can_cheat = (tick == tick_cheat) || (tick == (tick_cheat + 1));
            // //bool can_cheat = (tick == (tick_cheat + 1));
            // bool can_cheat = (tick == tick_cheat);
            // if ((grid[r][c] == '#') && !can_cheat)
            //     continue;

            int steps = costs[{r, c}] + 1;
            //cout << r << " " << c << " " << grid[r][c] << steps << endl;

            auto make_step = [&](int dr, int dc)
            { 
                if (costs.find({r + dr, c + dc}) == costs.end())
                    costs[{r + dr, c + dc}] = steps;
                else
                    costs[{r + dr, c + dc}] = min(steps, costs[{r + dr, c + dc}]);

                int r2 = r + dr; 
                int c2 = c + dc; 
                if ((r2 == 0) || (c2 == 0) || (r2 == (Rows - 1)) || (c2 == (Cols - 1))) return;    

                if (visited.find({r + dr, c + dc}) == visited.end()) front2.insert({r + dr, c + dc});
            };

            make_step( 1,  0);
            make_step(-1,  0);
            make_step( 0,  1);
            make_step( 0, -1);
        }

        front = front2;
    }

    return tick;
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int result = 0;
    int base   = run(input, -2, 'N') - 99;
    cout << base << endl;

    map<int, int> saves;
    for (auto t: aoc::range(0, base))
    {        
        cout << t << endl;

        {
            int tick = run(input, t, 'N');
            saves[base - tick] = saves[base - tick] + 1;
            result += (base > tick);
        }
        {
            int tick = run(input, t, 'S');
            saves[base - tick] = saves[base - tick] + 1;
            result += (base > tick);
        }
        {
            int tick = run(input, t, 'E');
            saves[base - tick] = saves[base - tick] + 1;
            result += (base > tick);
        }
        {
            int tick = run(input, t, 'W');
            saves[base - tick] = saves[base - tick] + 1;
            result += (base > tick);
        }
    }

    for (auto [k, v]: saves) cout << k << " " << v << endl; cout << endl;

    return result;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;
    return 0;
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
