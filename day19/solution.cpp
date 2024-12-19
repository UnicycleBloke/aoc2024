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
            matched = matched && (pattern[offset + i] == t[i]); 

        if (matched)
        {
            lengths.insert(offset + len);
            if (is_possible(pattern, offset + len, towels, lengths)) return true;
        }
    }

    return false;
}


uint64_t count_the_ways(const string pattern, size_t offset, const vector<string>& towels, map<size_t, size_t>& counts)
{
    auto start = pattern.size() - (offset + 1); 
    auto size  = offset + 1;

    uint64_t result{};
    for (const auto& t: towels)
    {       
        size_t len = t.size();
        if (size < len) continue;

        bool matched = true;
        for (auto i: aoc::range(len))
            matched = matched && (pattern[start + i] == t[i]); 
 
        if (matched)
            result += (size == len) ? 1 : counts[offset - len];
    }

    counts[offset] = result;
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
    uint32_t possible{};
    for (auto p: aoc::range(1U, input.size()))
    {
        // Count how many ways there are to match the tail of the required pattern, and 
        // increment the tail length until the whole pattern is matched. Could make this 
        // recursive and deal with the ordering auto-magically, but this is fine.
        map<size_t, size_t> counts;
        for (auto i: aoc::range(input[p].size()))
            count_the_ways(input[p], i, towels, counts);

        auto ways = counts[input[p].size() - 1U];
        count    += ways;
        possible += (ways > 0);
    } 

    cout << possible << endl;
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
