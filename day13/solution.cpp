#include "utils.h"


struct Game
{
    int64_t ax;
    int64_t ay;
    int64_t bx;
    int64_t by;
    int64_t px;
    int64_t py;
};
 

auto run_games(const vector<Game>& games, int64_t offset)
{
    int64_t total = 0; 

    // Each game involve solving a pair of simultaneous equations. So glad I took a moment to 
    // write down expressions for the final position. Immediately spotted the SE and did not 
    // waste time on the suggested simulation. Would have killed P2.
    //    
    // There is a unique solution but it may involve a non-integral number of moves for A or B.
    // We ignore those ones. I didn't explicity check for division by zero. Was expecting a nasty 
    // edge case but seem to have got away with it.
    for (const auto& [ax, ay, bx, by, prizex, prizey]: games)
    {
        auto px = prizex + offset;
        auto py = prizey + offset;

        auto num = px * ay - py * ax;
        auto den = bx * ay - by * ax;

        if ((num % den) == 0)
        {
            auto move_b = num / den;
            auto num    = px  - move_b * bx;
            if ((num % ax) == 0)
            {
                auto move_a = num / ax;

                total += move_a * 3 + move_b; 
            }
        }
    }

    return total;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress);
    
    vector<Game> games;

    size_t i{};
    while (i < lines.size())
    {
        auto a = aoc::parse_line<int64_t, int64_t>(R"(Button A: X\+(\d+), Y\+(\d+))", lines[i++]);
        auto b = aoc::parse_line<int64_t, int64_t>(R"(Button B: X\+(\d+), Y\+(\d+))", lines[i++]);
        auto p = aoc::parse_line<int64_t, int64_t>(R"(Prize: X=(\d+), Y=(\d+))", lines[i++]);

        auto [ax, ay] = a;
        auto [bx, by] = b;
        auto [px, py] = p;

        games.push_back(Game{ax, ay, bx, by, px, py});
    }

    auto p1 = run_games(games, 0);
    cout << "Part1: " << p1 << '\n';
    
    auto p2 = run_games(games, 10000000000000);
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
