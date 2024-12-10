#include "utils.h"


template <typename T, template<typename> typename Container>
auto count_trails(const T& input, int r, int c)
{
    const int Rows = input.size();
    const int Cols = input[0].size();

    auto in_bounds = [&](int row, int col)
    {
        return(row >= 0) && (row < Rows) && (col >= 0) && (col < Cols);
    };

    int heads = 0;

    using Head = pair<int, int>;
    auto append = [&](Container<Head>& cont, Head head)
    {
        if constexpr (is_same_v<Container<Head>, set<Head>>)
            cont.insert(head);
        else if constexpr (is_same_v<Container<Head>, vector<Head>>)
            cont.push_back(head);
    };

    Container<Head> trails;
    append(trails, Head{r, c});

    while (true)
    {        
        Container<Head> trails2;
        for (auto [r1, c1]: trails)
        {
            if (input[r1][c1] == '9') 
            {
                ++heads;
                continue;
            }

            int r2 = r1 + 1;
            int c2 = c1;
            if (in_bounds(r2, c2) && (input[r2][c2] - input[r1][c1] == 1))
                append(trails2, Head{r2, c2});

            r2 = r1 - 1;
            c2 = c1;
            if (in_bounds(r2, c2) && (input[r2][c2] - input[r1][c1] == 1))
                append(trails2, Head{r2, c2});

            r2 = r1;
            c2 = c1 + 1;
            if (in_bounds(r2, c2) && (input[r2][c2] - input[r1][c1] == 1))
                append(trails2, Head{r2, c2});

            r2 = r1;
            c2 = c1 - 1;
            if (in_bounds(r2, c2) && (input[r2][c2] - input[r1][c1] == 1))
                append(trails2, Head{r2, c2});
        }

        if (trails2.size() == 0) break;
        trails = trails2;
    }

    return heads;    
}


// Easy today, so I had a play with template template types to reduce code duplication.
template <typename T, template<typename> typename Container>
auto solve(const T& input)
{
    aoc::timer timer;

    int trails = 0;
    for (auto r: aoc::range(input.size()))
    {
        for (auto c: aoc::range(input[0].size()))
        {
            if (input[r][c] == '0')
                trails += count_trails<T, Container>(input, r, c);        
        }
    }

    return trails;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 

    // Use a set to avoid double duplicating locations on multiple trails.
    auto p1 = solve<decltype(lines), set>(lines);
    cout << "Part1: " << p1 << '\n';

    // Use a vector to duplicate locations on multiple trails.  
    auto p2 = solve<decltype(lines), vector>(lines);
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
