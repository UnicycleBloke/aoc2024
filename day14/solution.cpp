#include "utils.h"


struct Robot
{
    int px; 
    int py;
    int vx;
    int vy;
};


constexpr int XSize = 101;
constexpr int YSize = 103;


auto print(const vector<Robot>& robots)
{
    map<pair<int,int>, int> counts;
    for (auto i: aoc::range(robots.size()))
    {
        const auto& r = robots[i];
        counts[{r.px, r.py}] = counts[{r.px, r.py}] + 1;
    }    

    for (auto y: aoc::range(YSize))
    {
        for (auto x: aoc::range(XSize))
        {
            auto c = counts[{x, y}];
            char d = (c > 0) ? ('0' + c) : '.';
            cout << d;
        }
        cout << endl;
    }
    cout << endl;
}


auto safety(const vector<Robot>& robots)
{
    array<int64_t, 4> q{};

    for (auto [px, py, vx, vy]: robots)
    {
        q[0] += ((px < (XSize / 2)) && (py < (YSize / 2)));
        q[1] += ((px < (XSize / 2)) && (py > (YSize / 2)));
        q[2] += ((px > (XSize / 2)) && (py < (YSize / 2)));
        q[3] += ((px > (XSize / 2)) && (py > (YSize / 2)));
    }

    return q[0] * q[1] * q[2] * q[3];
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    vector<Robot> robots;
    for (auto [px, py, vx, vy]: input)
    {
        robots.push_back({px, py, vx, vy});
    }

    for (auto s: aoc::range(100))
    {
        for (auto& r: robots)
        {
            r.px = (((r.px + r.vx) % XSize) + XSize) % XSize;
            r.py = (((r.py + r.vy) % YSize) + YSize) % YSize;
        }
    }

    return safety(robots);
}


bool has_solid_block(const vector<Robot>& robots)
{
    map<pair<int,int>, int> counts;
    for (auto i: aoc::range(robots.size()))
    {
        const auto& r = robots[i];
        counts[{r.px, r.py}] = counts[{r.px, r.py}] + 1;
    }    

    constexpr int Size = 3;
    for (auto y: aoc::range(YSize - Size))
    {
        for (auto x: aoc::range(XSize - Size))
        {
            bool has_block = true;
            for (auto dy: aoc::range(Size))
                for (auto dx: aoc::range(Size))
                    has_block &= (counts[{x + dx, y + dy}] > 0);
            if (has_block) return true;
        }
    }

    return false;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    vector<Robot> robots;
    for (auto [px, py, vx, vy]: input)
    {
        robots.push_back({px, py, vx, vy});
    }

    int64_t result{0};
    auto advance = [&](int n)
    {
        for (auto& r: robots)
        {
            r.px = (((r.px + r.vx * n) % XSize) + XSize) % XSize;
            r.py = (((r.py + r.vy * n) % YSize) + YSize) % YSize;
        }
        result += n;
    };

    // Wow! Those were some vague requirements. 
    //
    // Spent ages thinking prime numbers something something greatest common divisor something.
    // Drawing grid after every step reveals a pattern:
    // - There is a marked vertical cluster at   steps = 82 + N * 101 (with my data)
    // - There is a marked horizontal cluster at steps = 47 + N * 103 (with my data)
    //
    // I reasoned that the first tree would be when the two clusters occurred at the same time.
    //
    // Solution:
    // - Initially advance 82 steps
    // - Advance in blocks of 101 steps
    // - Break when the number of steps % 103 is 47
    //
    // I need to learn how I can determine the values 82 and 47 from the input.
    advance(82);
    while (true)
    {
        advance(101);
        if ((result % 103) == 47) break;
    }

    // This is much slower method based on the assumption that the christmas tree image
    // will contain a solid NxN block of pixels. N=3 seems to be work. Slow but does take
    // out the visual inspection part.
    // while (true)
    // {
    //     advance(1);
    //     if (has_solid_block(robots)) break;
    //     cout << result << endl;
    // }

    //print(robots);
    return result;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<int,int,int,int>(filename, R"(p=(\d+),(\d+) v=(-?\d+),(-?\d+))");

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
