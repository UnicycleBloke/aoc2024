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


string make_expression(const string& w3, set<string>& wires, map<string, tuple<string, string, string>> expr)
{
    if (expr.find(w3) == expr.end()) return w3;
    const auto& [w1, w2, gate] = expr[w3];

    // if ((w1[0] == 'x') || (w1[0] == 'y')) wires.insert(w1);
    // if ((w2[0] == 'x') || (w2[0] == 'y')) wires.insert(w2);
    wires.insert(w1);
    wires.insert(w2);

    ostringstream os;
    ///cout << w3 << " = (" << w1 << " " << gate << " " << w2 << ")" << endl;
    os << "(" << make_expression(w1, wires, expr) << " " << gate << " " << make_expression(w2, wires, expr) << ")";
    return os.str();
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
    auto find_swap = [&](string v1)
    {
        if (swaps.find(v1) != swaps.end()) return swaps[v1];
        return v1;
    };

    auto add_swap = [&](string v1, string v2)    
    {
        cout << "Added swap: " << v1 << " " << v2 << endl;
        swaps[v1] = v2;
        swaps[v2] = v1;
    };

    add_swap("z07", "bjm");
    add_swap("z13", "hsw");
    add_swap("z18", "skf");
    add_swap("nvr", "wkr");

    auto find_gate = [&](string v1, string v2, string g)
    {
        auto t1 = make_tuple(v1, v2, g);
        auto t2 = make_tuple(v2, v1, g);
        if (logic.find(t1) != logic.end())
        {
            auto result = find_swap(logic[t1]);
            cout << "Found: " << v1 << " " << g << " " << v2 << " -> " << result << endl;
            return result;
        } 
        if (logic.find(t2) != logic.end())
        {
            auto result = find_swap(logic[t2]);
            cout << "Found: " << v2 << " " << g << " " << v1 << " -> " << result << endl;
            return result;
        } 

        cout << "ERROR: " << v1 << " " << g << " " << v2 << " -> ***" << endl;
        return string{"***"};
    };

    auto zbit = find_gate("x00", "y00", "XOR");
    auto cbit = find_gate("x00", "y00", "AND");
    if (zbit != "z00") cout << "Error z00\n";
    if (cbit.size() == 0) cout << "Error c00\n";

    int i = 1;
    while (i < 45)
    //for (auto i: aoc::range(1, 45))
    {
        char buffer[64];
        sprintf(buffer, "%02d", i);  
        string xbit = "x" + string(buffer); 
        string ybit = "y" + string(buffer); 
        string zbit = "z" + string(buffer); 

        cout << "INP: " << xbit << " " << ybit << " " << cbit << endl;

        auto xor1 = find_gate(xbit, ybit, "XOR");
        auto and1 = find_gate(xbit, ybit, "AND");
        auto xor2 = find_gate(xor1, cbit, "XOR");
        auto and2 = find_gate(xor1, cbit, "AND");
        auto or1  = find_gate(and1, and2, "OR");

        // if ((xor2 != zbit) && (and2 == zbit))
        // {
        //     add_swap(xor2, zbit);
        //     continue;
        // }
        // if ((xor2 != zbit) && (or1 == zbit))
        // {
        //     add_swap(or1, zbit);
        //     continue;
        // }

        cout << "ADD: " << xor1 << " " << xor2 << endl;
        cout << "CRY: " << and1 << " " << and2 << " " << or1 << endl;

        cbit = or1;

        cout << endl;
        ++i;
    } 

    for (auto [k, v]: swaps)
        cout << k << ",";
    cout << endl;

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
