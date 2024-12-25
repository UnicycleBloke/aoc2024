#include "utils.h"


struct Item
{
    int           size{};
    array<int, 5> cols{};
};


auto part1(const vector<Item>& keys, vector<Item>& locks)
{
    aoc::timer timer;

    int pairs = 0;
    for (const auto& key: keys)
    {
        for (const auto& lock: locks)
        {
            bool fits = true;
            for (auto c: aoc::range(5))
                fits &= ((key.cols[c] + lock.cols[c]) <= lock.size);
            pairs += fits;
        }
    }

    return pairs;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Allow); 

    vector<Item> keys;
    vector<Item> locks;

    int i = 0;
    while (i < lines.size())    
    {
        bool is_key = (lines[i] == ".....");

        Item item{};
        while (lines[i].size() > 0)
        {   
            ++item.size; 
            for (auto k: aoc::range(5))
                item.cols[k] += (lines[i][k] == '#');
            ++i;
        }
        while (lines[i].size() == 0) ++i;

        if (is_key) 
            keys.push_back(item); 
        else 
            locks.push_back(item);
    }

    auto p1 = part1(keys, locks);
    cout << "Part1: " << p1 << '\n';
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
