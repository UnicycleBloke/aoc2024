#include "utils.h"


// Represents one of the keypads used in the problem.
struct KeyBoard
{
    using Point = pair<int, int>;
    map<char, Point> keys;
    map<Point, char> keys2;
    int row;
    int col;
    int prow;
    int pcol;

    // Construct from an array of characters.
    KeyBoard(int rows, int cols, const char* data)
    {
        for (auto r: aoc::range(rows))
        {
            for (auto c: aoc::range(cols))
            {
                char key = data[cols * r + c];
                keys[key] = {r, c};
                keys2[{r, c}] = key;
                if (key == 'A')
                {
                    row = r;
                    col = c;
                }
            }
        }
        prow = row;
        pcol = col;
    }

    // Used to eliminate paths which pass through the forbidden space.
    bool is_valid_path(const string& moves)
    {
        int row1 = row;
        int col1 = col;
        for (char m: moves)
        {
            switch (m)
            {
                case '<': col1 -= 1; break;
                case '>': col1 += 1; break;
                case '^': row1 -= 1; break;
                case 'v': row1 += 1; break;
            }
            if (keys2[{row1, col1}] == '*') return false;
        }
        return true;
    }

    void move_to(char key)
    {
        auto [row2, col2] = keys[key];
        col = col2;
        row = row2;
    }

    // Enumerate all the possible paths from the current position to the new position.
    // The shortest distance is basically a Manhattan path, but we may have several places
    // in which to go left or right before reaching the end.
    vector<string> get_paths(char key)
    {
        ostringstream os;
        auto [row2, col2] = keys[key];

        // A straightforward Manhattan path.
        for (auto i: aoc::range(abs(col2 - col)))
            os << ((aoc::sgn(col2 - col) < 0) ? '<' : '>');
        for (auto i: aoc::range(abs(row2 - row)))
            os << ((aoc::sgn(row2 - row) < 0) ? '^' : 'v');

        // Permute to obtain all possibilities.
        vector<string> paths;
        string path = os.str();
        sort(path.begin(), path.end());
        do
        {
            if (is_valid_path(path))
                paths.push_back(path + "A");
        }
        while (next_permutation(path.begin(), path.end()));

        return paths;
    }
};


int number(const string& code)
{
    int result = 0;
    for (char c: code)
    {
        if (c == 'A') continue;
        result = result * 10 + (c - '0');
    }
    return result;
}


// Recursive evaluation of a sequence. There are relatively few sequence "atoms" (move from ButtonX to ButtonY),
// but these expand to much longer sequences for the next bot in the chain, and so on.
uint64_t find_min_length(const string& sequence, int depth, int max_depth, map<pair<int, string>, uint64_t>& cache)
{
    // Terminate the recursion.
    if (depth == max_depth) 
        return sequence.size(); 

    // Check the cache to see if we have evaluated this combination already.
    const auto iter = cache.find({depth, sequence});
    if (iter != cache.end())
        return iter->second; 

    uint64_t total_min_length{};

    // It helps that every sequence returns to the A key to tell the next bot to push, or 
    // we'd have to keep a separate Keyboard alive for each bot.
    KeyBoard dir{2, 3, "*^A<v>" };
    for (char c: sequence)
    {
        // Find all the possible sequences for the key.
        auto paths = dir.get_paths(c);
        dir.move_to(c);

        // Recurse to the next bot to find the ultimate length. 
        // Take the minimum of all of these.
        uint64_t min_length = numeric_limits<uint64_t>::max();
        for (const auto& p: paths)
        {
            uint64_t length = find_min_length(p, depth + 1, max_depth, cache);
            min_length = min(min_length, length);
        }
        total_min_length += min_length;
    }

    // Update the cache.
    cache[{depth, sequence}] = total_min_length;
    return total_min_length;
}


size_t find_min_length(const string& code, int max_depth)
{
    KeyBoard pad{4, 3, "789456123*0A" };

    vector<string> all_paths;
    all_paths.push_back("");

    // Build all top level sequences that the first bot would need to execute.
    for (char c: code)
    {
        auto paths = pad.get_paths(c);
        pad.move_to(c);

        vector<string> all_paths2;
        for (const auto& r: all_paths)
            for (const auto& p: paths)
                all_paths2.push_back(r + p);
        all_paths = all_paths2;
    }

    // Cache the total path length for a given sequence and depth to avoid repetition. 
    map<pair<int, string>, uint64_t> cache;

    // Try all the possible sequences to see which is the shortest.
    uint64_t min_length = numeric_limits<uint64_t>::max();
    for (const auto& path: all_paths)
    {        
        auto length = find_min_length(path, 0, max_depth, cache);
        min_length = min(min_length, length);
    }

    return min_length;
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;   
    size_t result{};

    for (auto code: input)
    {
        auto length = find_min_length(code, 2);
        result += length * number(code);
    } 

    return result;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;   
    size_t result{};

    for (auto code: input)
    {
        cout << code << endl;
        auto length = find_min_length(code, 25);
        result += length * number(code);
    } 

    return result;
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
