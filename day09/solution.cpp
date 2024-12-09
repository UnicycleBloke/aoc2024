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

    // for (auto i: blocks)
    //     cout << i;
    // cout << endl;

    int beg  = 0;
    int end  = blocks.size() - 1;
    int size = blocks.size();

    //while ((end > 0) && (beg < size))
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

    // cout << "b=" << beg << " e=" << end << endl;
    // cout << "b=" << blocks[beg] << " e=" << blocks[end] << endl;
    // cout << "_123456789_123456789_123456789_123456789_123456789" << endl;

    // for (auto i: blocks)
    //     cout << i;
    // cout << endl;

    // for (auto i: aoc::range(blocks.size()))
    // {
    //     if (blocks[i] == -1) 
    //         cout << '.';
    //     else 
    //         cout << blocks[i];
    // }
    // cout << endl;


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
    int id{}; // -1 is free;
    int size{};
    bool ignore{};
};


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    int         file{};    
    list<Block> blocks;

    size_t pos{};
    while (pos < input[0].size())
    {
        auto len = input[0][pos] - '0';
        if ((pos % 2) == 0)
        {
            blocks.push_back(Block{file, len});
            ++file;
        }
        else
        {
            blocks.push_back(Block{-1, len});
        }
        ++pos; 
    }

    for (const auto& b: blocks)
    {
        cout << b.id << " " << b.size << endl; 
    }


    //for (auto i: aoc::range(10))
    while (true)
    {
        auto rev = blocks.rbegin();
        while ((rev != blocks.rend()) && ((rev->id == -1) || rev->ignore))
            rev++;
        if (rev == blocks.rend()) break;
        //cout << "rev " << rev->id << " sz=" << rev->size << " end=" << (rev != blocks.rend()) << endl;

        auto fwd = blocks.begin();
        while ((fwd != blocks.end()) && ((fwd->id != -1) || (fwd->size < rev->size))) // || fwd->ignore)))
        {
            //cout << "fwd " << fwd->id << " sz=" << fwd->size << " end=" << (fwd != blocks.end()) <<  endl;
            fwd++;
        }
        //cout << "fwd " << fwd->id << " sz=" << fwd->size << " end=" << (fwd != blocks.end()) <<  endl;
        if (fwd == blocks.end())
        {  
            //cout << "skip file" << endl;
            rev->ignore = true;
            continue;
        }

        Block file = *rev;
        file.ignore = true;
        blocks.insert(fwd, file);

        rev->id     = -2;
        rev->ignore = true;

        fwd->size -= file.size;
        if (fwd->size == 0)
            blocks.erase(fwd);

        // for (const auto& b: blocks)
        // {
        //     char c = (b.id < 0) ? '.' : '0' + b.id;
        //     for (auto j: aoc::range(b.size))
        //         cout << c;
        // }
        //cout << endl;
    }

    int64_t result{};
    int     index{};
    for (const auto& b: blocks)
    {        
        for (auto j: aoc::range(b.size))
        {
            if (b.id >= 0) result += index * b.id;
            ++index; 
        }
    } 

    return result;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 
    //cout << lines[0].size();

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    
    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
}


int main(int argc, char** argv)
{
    aoc::timer timer;
    //try
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
    // catch (std::exception& e)
    // {
    //     cout << e.what();
    // }
}
