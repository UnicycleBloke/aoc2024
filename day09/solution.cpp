#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int         file{};    
    vector<int> blocks;

    size_t pos{};
    while (pos < input[0].size())
    {
        auto len = input[0][pos] - '0';
        if ((pos % 2) == 0)
        {
            for (auto i: aoc::range(len)) blocks.push_back(file); 
            ++file;
        }
        else
        {
           for (auto i: aoc::range(len)) blocks.push_back(-1);
        }
        ++pos; 
    }

    int beg  = 0;
    int end  = blocks.size() - 1;
    int size = blocks.size();

    while (end > beg)
    {
        while ((beg < size) && (blocks[beg] != -1))
            ++beg;            
        while ((end > 0) && (blocks[end] == -1))
            --end;

        while ((blocks[beg] == -1) && (blocks[end] != -1))
        {
            int temp    = blocks[beg];
            blocks[beg] = blocks[end];
            blocks[end] = temp;
            ++beg;
            --end;
        }
    }

    int64_t result{};
    for (auto i: aoc::range(blocks.size()))
    {
        if (blocks[i] == -1) break;
        result += i * blocks[i]; 
    } 

    return result;
}


struct Block
{
    size_t index{}; 
    size_t size{};
    size_t base{};
};


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    size_t        index{};    
    vector<Block> files;
    vector<Block> spaces;

    //files.reserve(50000);
    //spaces.reserve(50000);

    // Create the data
    size_t p{};
    size_t base{};
    while (p < input[0].size())
    {
        size_t size = input[0][p] - '0';
        if ((p % 2) == 0)
        {
            files.emplace_back(Block{index, size, base});
            ++index;
        }
        else
        {
            spaces.emplace_back(Block{0, size, base});
        }
        ++p; 
        base += size;
    }

    // Perform the defrag
    for (auto it = files.rbegin(); it != files.rend(); ++it)
    {
        auto& file = *it;
        for (auto& space: spaces)
        {
            if ((space.size >= file.size) && (space.base < file.base))
            {
                file.base   = space.base;
                space.base += file.size;
                space.size -= file.size;
            }
        }
    }

    // Calculate the result
    size_t total{};    
    for (auto& file: files)
    {
        total += file.index * (file.base * file.size + (file.size - 1) * file.size / 2);
    }

    return total;
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
