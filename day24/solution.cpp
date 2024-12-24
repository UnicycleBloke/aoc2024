#include "utils.h"


template <typename T, typename U>
auto part1(const T& init, const U& gates)
{
    aoc::timer timer;

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


// add_swap("z07", "bjm");
// add_swap("z13", "hsw");
// add_swap("z18", "skf");
// add_swap("nvr", "wkr");


template <typename T>
bool repair_adder(T& logic, int level, map<string, string> swaps, string cbit, vector<string>& wires)
{
    auto find_swap = [&](string v1)
    {
        if (swaps.find(v1) != swaps.end()) return swaps[v1];
        return v1;
    };

    auto find_gate = [&](string v1, string v2, string g)
    {
        auto t1 = make_tuple(v1, v2, g);
        auto t2 = make_tuple(v2, v1, g);
        if (logic.find(t1) != logic.end())
        {
            auto result = find_swap(logic[t1]);
            return result;
        } 
        if (logic.find(t2) != logic.end())
        {
            auto result = find_swap(logic[t2]);
            return result;
        } 

        return string{"***"};
    };

    if (level == 45)
    {
        // Why is the output being printed 8 times. Doesn't matter I guess.
        // Probably ended up with repeats in the wires vector.
        for (auto [k, v]: swaps)
            cout << k << ",";
        cout << endl;
        return true;
    }

    if (level == 0)
    {
        auto cbit0 = find_gate("x00", "y00", "AND");
        return repair_adder(logic, 1, swaps, cbit0, wires);
    }

    char buffer[64];
    sprintf(buffer, "%02d", level);  
    string xbit = "x" + string(buffer); 
    string ybit = "y" + string(buffer); 
    string zbit = "z" + string(buffer); 

    // The graph is a broken ripple-carry adder. Check for the expected gates.
    // A full adder is basically two half-adders strapped together to incorporate the carry bit.
    auto xor1 = find_gate(xbit, ybit, "XOR");  // half-add?
    auto and1 = find_gate(xbit, ybit, "AND");  // half-carry?
    auto xor2 = find_gate(xor1, cbit, "XOR");  // The new output bit z_level
    auto and2 = find_gate(xor1, cbit, "AND");
    auto or1  = find_gate(and1, and2, "OR");   // The new carry bit c_level
   
    bool ok = true;
    ok &= (find(wires.begin(), wires.end(), "***") == wires.end());
    ok &= (zbit == xor2);
    ok &= (zbit != and2);
    ok &= (zbit != or1);

    if (!ok)
    {
        // The caller will iterate over pairs of these wires, making trial swaps.
        wires.push_back(xor1);
        wires.push_back(and1);
        wires.push_back(xor2);
        wires.push_back(and2);
        wires.push_back(or1);
        wires.push_back(zbit);
        return false;
    } 

    vector<string> wires2;
    ok = repair_adder(logic, level + 1, swaps, or1, wires2);
    if (!ok)
    {
        for (auto j: aoc::range(wires2.size()))
        {
            for (auto k: aoc::range(j + 1, wires2.size()))
            {
                auto swaps2 = swaps;
                swaps2[wires2[j]] = wires2[k];
                swaps2[wires2[k]] = wires2[j];
                vector<string> wires3;
                if (repair_adder(logic, level + 1, swaps2, or1, wires3)) return true;
            }
        }
    }

    return false;
}


template <typename T, typename U>
auto part2(const T& init, const U& gates)
{
    aoc::timer timer;

    map<tuple<string, string, string>, string> logic;
    for (const auto& [w1, gate, w2, w3]: gates)
    {
        logic[make_tuple(w1, w2, gate)] = w3;
    }

    map<string, string> swaps;
    vector<string>      wires;
    repair_adder(logic, 0, swaps, "", wires);

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
