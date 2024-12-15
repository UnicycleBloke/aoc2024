#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    vector<string> grid;
    vector<string> moves;
    for (auto line: input)
    {
        if (line[0] == '#') grid.push_back(line);
        else moves.push_back(line);
    }

    uint32_t row{}, col{};

    // Find the size and initial position.
    auto rows = grid.size();
    auto cols = grid[0].size();
    for (auto r: aoc::range(rows))
    {
        for (auto c: aoc::range(cols))
        {
            if (grid[r][c] == '@')
            {
                row = r;
                col = c;
                break;
            }
        }
    }      

    auto do_move = [&](int dr, int dc)
    {
        auto row2 = row;
        auto col2 = col;

        // Keep looking until we find a space or hit a wall.
        while (true)
        {
            row2 += dr;
            col2 += dc;

            if (grid[row2][col2] == '#') 
            {
                return;
            } 
            else if (grid[row2][col2] == '.')
            {
                grid[row][col] = '.';
                grid[row2][col2] = 'O';
                row += dr;
                col += dc;
                grid[row][col] = '@';
                return;
            }
        }
    };

    for (auto move: moves)
    {
        for (auto m: move)
        {
            switch (m)
            {
                case '>': do_move( 0,  1); break;
                case '<': do_move( 0, -1); break;
                case '^': do_move(-1,  0); break;
                case 'v': do_move( 1,  0); break;
            }
        }
    }

    uint64_t gps{};
    for (auto r: aoc::range(rows))
    {
        for (auto c: aoc::range(cols))
        {
            if (grid[r][c] == 'O')
                gps += 100*r + c;
        }
    }              

    return gps;
}


bool can_move_horz(const vector<string>& grid, int dir, int r, int c)
{
    switch (grid[r][c + dir])
    {
        case '#': return false;
        case '.': return true;
        case '[': 
        case ']': return can_move_horz(grid, dir, r, c + dir);
    }
    return false;
}


bool can_move_vert(const vector<string>& grid, int dir, int r, int c)
{
    switch (grid[r + dir][c])
    {
        case '#': return false;
        case '.': return true;
        case '[': return can_move_vert(grid, dir, r + dir, c) && can_move_vert(grid, dir, r + dir, c + 1);
        case ']': return can_move_vert(grid, dir, r + dir, c) && can_move_vert(grid, dir, r + dir, c - 1);
    }
    return false;
}


void do_move_horz(vector<string>& grid, int dir, int r, int c)
{
    auto temp = grid[r][c];
 
    if (grid[r][c + dir] != '.')
        do_move_horz(grid, dir, r, c + dir);
    grid[r][c + dir] = temp;
 
    grid[r][c] = '.';    
}


void do_move_vert(vector<string>& grid, int dir, int r, int c)
{
    auto temp = grid[r][c];

    if (grid[r + dir][c] == '[')
    {
        do_move_vert(grid, dir, r + dir, c);
        do_move_vert(grid, dir, r + dir, c + 1);
    }
    else if (grid[r + dir][c] == ']')
    {
        do_move_vert(grid, dir, r + dir, c);
        do_move_vert(grid, dir, r + dir, c - 1);
    }

    grid[r + dir][c] = temp;
    grid[r][c] = '.';    
}    


void perform_move_horz(vector<string>& grid, int dir, int& row, int& col)
{
    if (can_move_horz(grid, dir, row, col))
    {
        do_move_horz(grid, dir, row, col);
        col += dir; 
    }
}


void perform_move_vert(vector<string>& grid, int dir, int& row, int& col)
{
    if (can_move_vert(grid, dir, row, col))
    {
        do_move_vert(grid, dir, row, col);
        row += dir; 
    }
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    // Create the expanded grid.
    vector<string> grid;
    vector<string> moves;
    for (auto line: input)
    {
        if (line[0] == '#')
        {
            ostringstream os;
            for (auto c: line)
            {
                switch (c)
                {
                    case '#': os << "##"; break;
                    case 'O': os << "[]"; break;
                    case '.': os << ".."; break;
                    case '@': os << "@."; break;
                }
            }
            grid.push_back(os.str());
        }
        else 
        {
            moves.push_back(line);
        }
    }

    int row{}, col{};

    // Find the size and initial position.
    auto rows = grid.size();
    auto cols = grid[0].size();
    for (auto r: aoc::range(rows))
    {
        for (auto c: aoc::range(cols))
        {
            if (grid[r][c] == '@')
            {
                row = r;
                col = c;
                break;
            }
        }
    }      

    for (auto move: moves)
    {
        for (auto m: move)
        {
            switch (m)
            {
                case '>': perform_move_horz(grid,  1, row, col); break;
                case '<': perform_move_horz(grid, -1, row, col); break;
                case '^': perform_move_vert(grid, -1, row, col); break;
                case 'v': perform_move_vert(grid,  1, row, col); break;
            }
        }
    }              

    uint64_t gps{};
    for (auto r: aoc::range(rows))
    {
        for (auto c: aoc::range(cols))
        {
            if (grid[r][c] == '[')
                gps += 100*r + c;
        }
    }              

    return gps;
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
