#include "utils.h"


bool is_possible(const char* pattern, size_t size, const vector<string>& towels)
{
    //cout << "poss? " << pattern << " " << size << endl;
    if (size == 0) return true;

    set<size_t> lengths;
    for (const auto& t: towels)
    {       
        size_t len = t.size();
        if (lengths.find(len) != lengths.end())
        {
            cout << "skip " << t << endl;
            continue;
        } 
        if (size < len) continue;

        bool matched = true;
        for (auto i: aoc::range(len))
        {
            matched = matched && (pattern[i] == t[i]); 
        }

        if (matched)
        {
            lengths.insert(len);
            if (is_possible(pattern + len, size - len, towels)) return true;
        }
    }

    return false;
}


// bool is_possible2(const char* pattern, size_t size, const vector<string>& towels, set<size_t>& lengths)
// {
//     //cout << "poss? " << pattern << " " << size << endl;
//     if (size == 0) return true;

//     for (const auto& t: towels)
//     {       
//         size_t len = t.size();
//         if (size < len) continue;

//         bool matched = true;
//         for (auto i: aoc::range(len))
//         {
//             matched = matched && (pattern[i] == t[i]); 
//         }

//         if (matched)
//         {

//             if (is_possible2(pattern + len, size - len, towels, lengths)) return true;
//         }
//     }

//     unmatched.insert(size);
//     return false;
// }


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    auto towels = aoc::split(input[0], ",");
    sort(towels.begin(), towels.end(), [](auto a, auto b){ return a.size() > b.size(); });
    cout << "towels " << towels.size() << endl;
    for (auto t: towels) cout << t << " "; cout << endl;

    set<size_t> sizes;
    for (auto t: towels) sizes.insert(t.size());
    for (auto s: sizes) cout << s << " "; cout << endl;

    uint32_t count{};
    for (auto p: aoc::range(1U, input.size()))
    {
        cout << "pattern " << input[p] << endl;
        const char* pattern = input[p].c_str();
        size_t length = input[p].size();

        //count += is_possible(pattern, length, towels);

        // This is much quicker omits cases where the towel straddles the midpoint.
        // size_t half = length / 2;
        // auto poss1 = is_possible(pattern, half, towels);
        // if (poss1)
        // {
        //     auto poss2 = is_possible(pattern + half, length - half, towels);
        //     count += (poss1 && poss2);
        // }
        // cout << count << endl;

        auto match_pattern = [&]()
        {
            for (auto offset: sizes)
            {
                size_t len1 = length / 3 - offset;
                auto poss1 = is_possible(pattern, len1, towels);
                if (poss1)
                {
                    cout << "poss1" << endl;
                    for (auto offset2: sizes)
                    {
                        size_t len2 = (length - len1) / 2 - offset2;
                        auto poss2 = is_possible(pattern + len1, len2, towels);
                        if (poss2)
                        {
                            cout << "poss2" << endl;
                            auto poss3 = is_possible(pattern + len1 + len2, length - len1 - len2, towels);
                            if (poss3)
                            {
                                cout << "poss3" << endl;
                                ++count;
                                return;
                            }
                        }
                    }
                }
            }
        };     
        match_pattern();   
        cout << count << endl;

        // for (auto offset: sizes)
        // {
        //     size_t half = length / 2 - offset;
        //     auto poss1 = is_possible(pattern, half, towels);
        //     if (poss1)
        //     {
        //         auto poss2 = is_possible(pattern + half, length - half, towels);
        //         if (poss2)
        //         {
        //             cout << "possible" << endl;
        //             ++count;
        //             break;
        //         }
        //     }
        //     cout << count << endl;
        // }

        // auto poss = is_possible2(pattern, length / 2, towels, unmatched);
        // cout << "poss=" << poss << endl;
        // for (auto u: unmatched) cout << u << " "; cout << endl;
        //count += poss; 
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
