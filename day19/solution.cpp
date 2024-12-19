#include "utils.h"


bool is_possible(const string pattern, size_t offset, const vector<string>& towels, set<size_t>& lengths)
{
    if (offset == pattern.size()) return true;
    auto size = pattern.size() - offset;

    for (const auto& t: towels)
    {       
        size_t len = t.size();
        if (size < len) continue;
        if (lengths.find(offset + len) != lengths.end()) continue;

        bool matched = true;
        for (auto i: aoc::range(len))
        {
            matched = matched && (pattern[offset + i] == t[i]); 
        }

        if (matched)
        {
            lengths.insert(offset + len);
            if (is_possible(pattern, offset + len, towels, lengths)) return true;
        }
    }

    return false;
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    auto towels = aoc::split(input[0], ",");
    sort(towels.begin(), towels.end(), [](auto a, auto b){ return a.size() > b.size(); });
    //cout << "towels " << towels.size() << endl;
    //for (auto t: towels) cout << t << " "; cout << endl;

    set<size_t> sizes;
    for (auto t: towels) sizes.insert(t.size());
    //for (auto s: sizes) cout << s << " "; cout << endl;

    uint32_t count{};
    for (auto p: aoc::range(1U, input.size()))
    {
        set<size_t> lengths;
        count += is_possible(input[p], 0, towels, lengths);
    } 

    return count;
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
