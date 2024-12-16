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

    // for (auto r: aoc::range(kRows))
    // {
    //     for (auto c: aoc::range(kCols))
    //     {
    //         if (input[r][c] == 'S')
    //         {
    //         } 
    //     }
    // }
    int  row = kRows - 2;
    int  col = 1;
    char dir = 'E';

    map<pair<int, int>, int> scores;
    scores[{row, col}] = 0;

    set<tuple<int, int, char>> visited;

    set<tuple<int, int, char>> front;
    front.insert(make_tuple(row, col, dir));

    while (front.size() > 0)
    {
        // cout << "while" << endl;
        // for (auto [r, c, d]: front)
        //     cout << r << " " << c << " " << d << endl;

        set<tuple<int, int, char>> front2;
        for (const auto& f: front)
        {
            visited.insert(f);

            const auto [r, c, d] = f;
            if (input[r][c] == 'E') 
            {
                cout << r << " " << c << " " << d << " EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << endl;
                int x = r;
                int y = c;
                auto p = make_pair(x, y);
                int score = scores[p];
                cout << score << endl;
            }
            
            {
                auto ds = dirs(d);
                auto [dr, dc] = ds;
                if (input[r + dr][c + dc] != '#')
                {
                    auto t = make_tuple(r + dr, c + dc, d);
                    if (visited.find(t) == visited.end())
                        front2.insert(t);

                    auto fpos = make_pair(r, c);    
                    auto tpos = make_pair(r + dr, c + dc);    
                    if (scores[tpos] == 0)                        
                        scores[tpos] = scores[fpos] + 1;
                    else
                        scores[tpos] = min(scores[tpos], scores[fpos] + 1);                        
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

                    auto fpos = make_pair(r, c);    
                    auto tpos = make_pair(r + dr, c + dc);    
                    if (scores[tpos] == 0)                        
                        scores[tpos] = scores[fpos] + 1001;
                    else
                        scores[tpos] = min(scores[tpos], scores[fpos] + 1001);                        
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

                    auto fpos = make_pair(r, c);    
                    auto tpos = make_pair(r + dr, c + dc);    
                    if (scores[tpos] == 0)                        
                        scores[tpos] = scores[fpos] + 1001;
                    else
                        scores[tpos] = min(scores[tpos], scores[fpos] + 1001);                        
                }
            }
        }

        front = front2;
    }

    for (auto [pos, score]: scores)
    {
        auto [row, col] = pos;
        cout << row << " " << col << " " << score << endl;
    }

    cout << scores[make_pair(1, kCols - 2)] << endl;

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
