#include "utils.h"


template <typename T>
auto part1(const T& input, int rows, int cols)
{
    aoc::timer timer;

    set<tuple<int, int>> nodes;

    for (auto& [freq, locs]: input)
    {
        for (auto i: aoc::range(locs.size()))
        {
            auto& [ri, ci] = locs[i];
            for (auto j: aoc::range(i + 1, locs.size()))
            {
                const auto& [rj, cj] = locs[j];

                int rd = rj - ri;
                int cd = cj - ci;

                auto insert = [&](int r, int c)
                {
                    if ((r >= 0) && (r < rows) && (c >= 0) && (c < cols))
                        nodes.insert(make_tuple(r, c));
                };

                insert(rj + rd, cj + cd);
                insert(ri - rd, ci - cd);
            }
        }
    }

    return nodes.size();
}


template <typename T>
auto part2(T& input, int rows, int cols)
{
    aoc::timer timer;

    set<tuple<int, int>> nodes;

    for (auto& [freq, locs]: input)
    {
        for (auto i: aoc::range(locs.size()))
        {
            auto& [ri, ci] = locs[i];
            for (auto j: aoc::range(i + 1, locs.size()))
            {
                const auto& [rj, cj] = locs[j];

                int rd = rj - ri;
                int cd = cj - ci;

                auto insert = [&](int r, int c)
                {
                    if ((r >= 0) && (r < rows) && (c >= 0) && (c < cols))
                    {
                        nodes.insert(make_tuple(r, c));
                        return true;
                    }
                    return false;
                };

                //insert(rj + rd, cj + cd);
                int row = rj;
                int col = cj;
                while (insert(row, col))
                {
                    row -= rd;
                    col -= cd;
                }

                //insert(ri - rd, ci - cd);
                row = ri;
                col = ci;
                while (insert(row, col))
                {
                    row += rd;
                    col += cd;
                }
            }
        }
    }

    return nodes.size();
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 
    int rows = lines.size();
    int cols = lines[0].size();

    map<char, vector<pair<int, int>>> freqs;

    for (auto r: aoc::range(rows))
    {
        for (auto c: aoc::range(cols))
        {
            if (lines[r][c] == '.') continue;

            auto& locs = freqs[lines[r][c]];
            locs.push_back(make_pair(r, c));
        }
    }

    auto p1 = part1(freqs, rows, cols);
    cout << "Part1: " << p1 << '\n';
    
    auto p2 = part2(freqs, rows, cols);
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
