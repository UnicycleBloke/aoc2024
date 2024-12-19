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


uint64_t is_possible2(const string pattern, size_t offset, const vector<string>& towels, map<size_t, size_t>& counts)
{
    if (offset == pattern.size()) return 1;
    if (counts.find(offset) != counts.end()) return counts[offset];

    auto size = pattern.size() - offset;

    set<size_t> lengths;

    uint64_t result{};
    for (const auto& t: towels)
    {       
        size_t len = t.size();
        if (size < len) continue;
        // Already matched with this length
        if (lengths.find(len) != lengths.end()) continue;

        bool matched = true;
        for (auto i: aoc::range(len))
        {
            matched = matched && (pattern[offset + i] == t[i]); 
        }

        if (matched)
        {
            lengths.insert(len);
            result += is_possible2(pattern, offset + len, towels, counts);
        }
    }

    return result;
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    auto towels = aoc::split(input[0], ",");
    sort(towels.begin(), towels.end(), [](auto a, auto b){ return a.size() > b.size(); });

    uint32_t count{};
    for (auto p: aoc::range(1U, input.size()))
    {
        // Memo for the lengths we already matched, to avoid pointless repetition.
        set<size_t> lengths;
        count += is_possible(input[p], 0, towels, lengths);
    } 

    return count;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    auto towels = aoc::split(input[0], ",");
    sort(towels.begin(), towels.end(), [](auto a, auto b){ return a.size() > b.size(); });

    uint64_t count{};
    for (auto p: aoc::range(1U, input.size()))
    {
        set<size_t> lengths;
        if (is_possible(input[p], 0, towels, lengths))
        {
            map<size_t, size_t> counts;
            count += is_possible2(input[p], 0, towels, counts);
            cout << count << endl;
        }
    } 

    return count;
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
