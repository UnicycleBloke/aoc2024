#include "utils.h"


int run(vector<string>& grid)
{
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

    while (front.size() > 0)
    {        
        set<pair<int, int>> front2;
        for (auto [r, c]: front)
        {
            visited.insert({r, c});

            if ((grid[r][c] == '#'))
                continue;

            int steps = costs[{r, c}] + 1;

            auto make_step = [&](int dr, int dc)
            { 
                if ((grid[r + dr][c + dc] == '#'))
                    return;

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

    // for (int r1: aoc::range(Rows))
    // {
    //     for (int c1: aoc::range(Cols))
    //     {
    //         cout << grid[r1][c1] << costs[{r1, c1}] << "  ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    set<tuple<int,int,int,int>> cheats;

    for (int r1: aoc::range(Rows))
    {
        for (int c1: aoc::range(Cols))
        {
            if (grid[r1][c1] != '#')
            {
                int cost1 = costs[{r1, c1}];
                for (int rd: aoc::range(-2, 2+1))
                {
                    for (int cd: aoc::range(-2, 2+1))
                    {
                        int cheat = abs(rd) + abs(cd);
                        if (cheat <= 2)
                        {
                            auto r2 = r1 + rd;
                            auto c2 = c1 + cd;
                            if ((r2 >= 0) && (c2 >= 0) && (r2 < Rows) && (c2 < Cols) && (grid[r2][c2] != '#'))
                            {
                                int cost2 = costs[{r2, c2}];
                                if (cost2 > (cost1 + cheat + 99))
                                {
                                    cheats.insert({r1, c1, r2, c2});
                                    //cout << cost1 << " " << cost2 << " " << (cost2 - cost1) << ": " << result << endl;
                                }
                            }    
                        }
                    }
                }
            }
        }
    }

    return cheats.size();
}


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
    return run(input);
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
