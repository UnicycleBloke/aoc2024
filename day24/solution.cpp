#include "utils.h"


template <typename T, typename U>
auto part1(const T& init, const U& gates)
{
    //aoc::timer timer;

    map<string, int> values;
    for (const auto& [wire, value]: init) 
    {
        values[wire] = value;
    }

    bool found = true;
    while (found)
    {
        found = false;
        for (const auto& [w1, gate, w2, w3]: gates)
        {
            if (values.find(w1) == values.end()) continue;
            if (values.find(w2) == values.end()) continue;
            if (values.find(w3) != values.end()) continue;

            found = true;
            if (gate[0] == 'A') values[w3] = values[w1] & values[w2];
            if (gate[0] == 'O') values[w3] = values[w1] | values[w2];
            if (gate[0] == 'X') values[w3] = values[w1] ^ values[w2];
        }
    }

    uint64_t result = 0;
    for (auto it = values.rbegin(); it != values.rend(); ++it)    
    {
        const auto& [wire, value] = *it;
        if (wire[0] == 'z')
            result = result * 2 + value;
    }

    return result;
}


template <typename T, typename U>
auto part2(const T& init, const U& gates)
{
    aoc::timer timer;

    for (auto i: aoc::range(45))
    {
        uint64_t value = uint64_t{1} << i;

        array<string, 45> x = 
        { 
            "x00", "x01", "x02", "x03", "x04", "x05", "x07", "x08", "x09", 
            "x10", "x11", "x12", "x13", "x14", "x15", "x17", "x18", "x19", 
            "x20", "x21", "x22", "x23", "x24", "x25", "x27", "x28", "x29", 
            "x30", "x31", "x32", "x33", "x34", "x35", "x37", "x38", "x39", 
            "x40", "x41", "x42", "x43", "x44" 
        }; 
        array<string, 45> y = 
        { 
            "y00", "y01", "y02", "y03", "y04", "y05", "y07", "y08", "y09", 
            "y10", "y11", "y12", "y13", "y14", "y15", "y17", "y18", "y19", 
            "y20", "y21", "y22", "y23", "y24", "y25", "y27", "y28", "y29", 
            "y30", "y31", "y32", "y33", "y34", "y35", "y37", "y38", "y39", 
            "y40", "y41", "y42", "y43", "y44" 
        }; 

        vector<tuple<string, int>> init2;
        for (auto j: aoc::range(45))
        {
            init2.push_back({x[j], (i == j)});
            init2.push_back({y[j], (i == j)});
        }

        cout << i << " " << value << " " << part1(init2, gates) << endl;
    }

    return 0;
}


void run(const char* filename)
{
    auto init = aoc::read_lines<string, int>(filename, R"(([\w\d]+): (\d+))");
    auto gates = aoc::read_lines<string, string, string, string>(filename, R"(([\w\d]+) (\w+) ([\w\d]+) -> ([\w\d]+))");

    auto p1 = part1(init, gates);
    cout << "Part1: " << p1 << '\n';

    auto p2 = part2(init, gates);
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
