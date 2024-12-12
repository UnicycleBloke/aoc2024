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

            // Simple count of all the plot boundaries with a different crop on the other side. 
            for (auto [row, col]: region.plots)
            {
                char cx = input[row][col];
                region.fences += input[row-1][col]   != cx;
                region.fences += input[row+1][col]   != cx;
                region.fences += input[row  ][col-1] != cx;
                region.fences += input[row  ][col+1] != cx;
            }

            // Count the corners of the region, which is the same as the number of edges. 
            set<pair<size_t, size_t>> corners;
            set<pair<size_t, size_t>> specials;
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

                auto is_special = [&](size_t r0, size_t c0, int rdir, int cdir)
                {
                    char cx  = input[r0][c0];
                    char cn  = input[r0+rdir][c0];
                    char cw  = input[r0][c0+cdir];
                    char cnw = input[r0+rdir][c0+cdir];

                    if (region.plots.find({r0+rdir, c0+cdir}) != region.plots.end())
                    {
                        return ((cx != cn) && (cx != cw) && (cx == cnw));
                    }
                    return false;
                };

                // Simple test to determine if any of the four corners of a plot in the region are 
                // corners of the region. Keep these in a set to avoid double counting.
                if (is_corner(row, col, +1, +1)) corners.insert({row+1, col+1});
                if (is_corner(row, col, +1, -1)) corners.insert({row+1, col});
                if (is_corner(row, col, -1, +1)) corners.insert({row, col+1});
                if (is_corner(row, col, -1, -1)) corners.insert({row, col});

                // Special case where two of a region's corners kiss. In this case we do need to count
                // the location twice. I was thinking about how to walk the perimeter and count turns,
                // but this turned out to be much simpler.
                if (is_special(row, col, +1, +1)) specials.insert({row+1, col+1});
                if (is_special(row, col, +1, -1)) specials.insert({row+1, col});
                if (is_special(row, col, -1, +1)) specials.insert({row, col+1});
                if (is_special(row, col, -1, -1)) specials.insert({row, col});
            }
            region.corners = corners.size() + specials.size();

            regions.push_back(region);
        }
    }

    size_t result1{};
    size_t result2{};
    for (const auto& reg: regions)
    {
        result1 += reg.fences * reg.plots.size();
        result2 += reg.corners * reg.plots.size();
    }
    cout << "Part1: " << result1 << endl;
    cout << "Part2: " << result2 << endl;

    return result2;
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
