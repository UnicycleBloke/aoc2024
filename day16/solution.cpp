#include "utils.h"


char turn_left(char dir)
{
    switch (dir)
    {
        case 'N': return 'W';
        case 'W': return 'S';
        case 'S': return 'E';
        case 'E': return 'N';
    }
    return 'N';
}


char turn_right(char dir)
{
    switch (dir)
    {
        case 'N': return 'E';
        case 'E': return 'S';
        case 'S': return 'W';
        case 'W': return 'N';
    }
    return 'N';
}


pair<int, int> dirs(char dir)
{
    switch (dir)
    {
        // drow, dcol
        case 'N': return make_pair(-1,  0);
        case 'W': return make_pair( 0, -1);
        case 'S': return make_pair( 1,  0);
        case 'E': return make_pair( 0,  1);
    }
    return make_pair(-1,  0);
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    const auto kRows = input.size();
    const auto kCols = input[0].size();

    int  row = kRows - 2;
    int  col = 1;
    for (auto r: aoc::range(kRows))
    {
        for (auto c: aoc::range(kCols))
        {
            if (input[r][c] == 'S')
            {
                row = r;
                col = c;
                break;
            } 
        }
    }
    char dir = 'E';

    map<tuple<int, int, char>, int> scores;
    scores[make_tuple(row, col, dir)] = 0;

    set<tuple<int, int, char>> visited;

    set<tuple<int, int, char>> front;
    front.insert(make_tuple(row, col, dir));

    while (front.size() > 0)
    {
        set<tuple<int, int, char>> front2;
        for (const auto& f: front)
        {
            visited.insert(f);
            const auto [r, c, d] = f;
            
            {
                auto ds = dirs(d);
                auto [dr, dc] = ds;
                if (input[r + dr][c + dc] != '#')
                {
                    auto t = make_tuple(r + dr, c + dc, d);
                    if (visited.find(t) == visited.end())
                        front2.insert(t);

                    if (scores[t] == 0)                        
                        scores[t] = scores[f] + 1;
                    else
                        scores[t] = min(scores[t], scores[f] + 1);                        
                }
            }

            {
                auto ds = dirs(turn_left(d));
                auto [dr, dc] = ds;
                if (input[r + dr][c + dc] != '#')
                {
                    auto t = make_tuple(r + dr, c + dc, turn_left(d));
                    if (visited.find(t) == visited.end())
                        front2.insert(t);

                    if (scores[t] == 0)                        
                        scores[t] = scores[f] + 1001;
                    else
                        scores[t] = min(scores[t], scores[f] + 1001);                        
                }

            }

            {
                auto ds = dirs(turn_right(d));
                auto [dr, dc] = ds;
                if (input[r + dr][c + dc] != '#')
                {
                    auto t = make_tuple(r + dr, c + dc, turn_right(d));
                    if (visited.find(t) == visited.end())
                        front2.insert(t);

                    if (scores[t] == 0)                        
                        scores[t] = scores[f] + 1001;
                    else
                        scores[t] = min(scores[t], scores[f] + 1001);                        
                }
            }
        }

        front = front2;
    }

    int result = 0x7FFF'FFFF;
    for (auto [pos, score]: scores)
    {
        auto [row, col, dir] = pos;
        cout << row << " " << col << " " << dir << " " << score << endl;
        if (input[row][col] == 'E')
            result = min(result, score);
    }

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
