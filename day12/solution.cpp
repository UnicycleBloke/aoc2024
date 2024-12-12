#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    auto rows = input.size();
    auto cols = input[0].size();

    struct Region
    {
        char   crop{};
        size_t fences{};
        size_t corners{};
        set<pair<size_t, size_t>> plots;
    };
    vector<Region> regions;

    auto visited = input;
    for (auto r: aoc::range(1U, rows - 1U))
    {
        for (auto c: aoc::range(1U, cols - 1U))
        {
            char crop = input[r][c];
            if (visited[r][c] == '.') continue;
            visited[r][c] = '.';

            Region region;
            region.crop = crop;
            region.plots.insert({r, c});

            set<pair<size_t, size_t>> plots;
            plots.insert({r, c});
            while (plots.size() > 0)
            {
                set<pair<size_t, size_t>> plots2;

                auto grow_region = [&](auto row, auto col) 
                {
                    char crop2 = visited[row][col];
                    if (crop2 == crop)
                    {
                        // Extend the region
                        visited[row][col] = '.';
                        plots2.insert({row, col});                        
                        region.plots.insert({row, col});
                    }
                };

                for (auto [r2, c2]: plots)
                {
                    grow_region(r2-1, c2);
                    grow_region(r2+1, c2);
                    grow_region(r2,   c2-1);
                    grow_region(r2,   c2+1);
                }

                plots = plots2;
            }

            for (auto [row, col]: region.plots)
            {
                char cx = input[row][col];
                region.fences += input[row-1][col]   != cx;
                region.fences += input[row+1][col]   != cx;
                region.fences += input[row  ][col-1] != cx;
                region.fences += input[row  ][col+1] != cx;
            }

            set<pair<size_t, size_t>> corners;
            for (auto [row, col]: region.plots)
            {
                auto is_corner = [&](size_t r0, size_t c0, int rdir, int cdir)
                {
                    char cx  = input[r0][c0];
                    char cn  = input[r0+rdir][c0];
                    char cw  = input[r0][c0+cdir];
                    char cnw = input[r0+rdir][c0+cdir];
                    // Not a corner
                    if ((cx == cn) && (cx == cw) && (cx == cnw)) return false;
                    if ((cx == cn) && (cx != cw) && (cx != cnw)) return false;
                    if ((cx != cn) && (cx == cw) && (cx != cnw)) return false;
                    return true;
                };

                auto is_nw_corner = [&](size_t r0, size_t c0)
                {
                    char cx  = input[r0][c0];
                    char cn  = input[r0-1][c0];
                    char cw  = input[r0][c0-1];
                    char cnw = input[r0-1][c0-1];
                    // Not a corner
                    if ((cx == cn) && (cx == cw) && (cx == cnw)) return false;
                    if ((cx == cn) && (cx != cw) && (cx != cnw)) return false;
                    if ((cx != cn) && (cx == cw) && (cx != cnw)) return false;
                    return true;
                };

                if (is_corner(row, col, +1, +1)) corners.insert({row+1, col+1});
                if (is_corner(row, col, +1, -1)) corners.insert({row+1, col});
                if (is_corner(row, col, -1, +1)) corners.insert({row, col+1});
                if (is_corner(row, col, -1, -1)) corners.insert({row, col});
            }
            region.corners = corners.size();
            cout << region.crop << " p=" << region.plots.size() << " f=" << region.fences << " c=" << region.corners << endl;

            regions.push_back(region);
        }
    }
//        .... 
// RRRRIIC.FF.
// RRRRII...F.
// VVRRRC.FFF.
// VVRCCC.FFF.
// VVVVCJ..F..
// VVIVCCJ....
// VVIIICJJEE
// MIIIIIJJEE
// MIIISIJEEE
// MMMISSJEEE


    size_t result{};
    for (const auto& reg: regions)
    {
        //result += reg.fences * reg.plots.size();
        result += reg.corners * reg.plots.size();
    }

    return result;
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
    auto grid  = aoc::make_grid_with_border(lines, 1, '.');

    auto p1 = part1(grid);
    cout << "Part1: " << p1 << '\n';

    auto p2 = part2(grid);
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
