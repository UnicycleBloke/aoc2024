#include "utils.h"


struct State
{
    int64_t a{};
    int64_t b{};
    int64_t c{};
    vector<int> prog;

    int64_t pc{};
    vector<int> dump;
    int64_t loops{};

    void run()
    {
        while ((pc < prog.size()) && (prog.size() > dump.size()) && (loops < 100))
        {
            ++loops;
            process();
        }     
    }

    void process()
    {
        auto opcode  = prog[pc++];
        auto literal = prog[pc++];

        auto combo = literal;
        switch (literal)
        {
            case 4: combo = a; break;
            case 5: combo = b; break;
            case 6: combo = c; break;
        }

        switch (opcode)
        {
            case 0: // adv - division
                a = a / (1 << combo);
                //cout << "a = a / (1 << combo) = a / " << (1 << combo) << " = " << a << endl;
                break;
            case 1: // bxl 
                b = b ^ literal;
                //cout << "b = b ^ " << literal << " = " << b << endl;
                break;
            case 2: // bst
                b = combo % 8;
                //cout << "b = " << b << endl;
                break;
            case 3: // jnz
                if (a != 0)
                    pc = literal;
                //cout << " a=" << a << "  b=" << b << "  c=" << c << endl;
                break;
            case 4: // bxc
                b = b ^ c;
                //cout << "b = b ^ c = " << b << endl;
                break;
            case 5: // out
                dump.push_back(combo % 8);
                //cout << "dump " << (combo % 8) << endl;
                break;
            case 6: // bdv
                b = a / (1 << combo);
                //cout << "b = a / (1 << combo) = a / " << (1 << combo) << " = " << b << endl;
                break;
            case 7: // cdv
                c = a / (1 << combo);
                //cout << "c = a / (1 << combo) = a / " << (1 << combo) << " = " << c << endl;
                break;
        } 
    }
};


template <typename T>
auto part1(T input)
{
    aoc::timer timer;

    input.run();
    for (auto c: input.dump)
        cout << c << ",";
    cout << endl;

    return 0;
}



vector<int> to_octal(int64_t value)
{
    std::vector<int> result;
    do 
    {
       result.push_back(value % 8);
       value = value >> 3;
    }
    while (value);
    r::reverse(result);
    return result;
}


int64_t from_octal(const vector<int>& value)
{
    int64_t result{};
    for (auto v: value)
        result = (result << 3) + v;
    return result << 3;
}


void print(const std::vector<int>& v)
{
    for (int e: v) 
    {
        std::cout << " " << e;
    }
    std::cout << std::endl;
}


template <typename T>
auto part2(T input)
{
    aoc::timer timer;

    // vector should be sorted at the beginning.
    //std::vector<int> prog = { 0, 3, 5, 4, 3, 0 };
    std::vector<int> prog = { 2, 4, 1, 5, 7, 5, 1, 6, 0, 3, 4, 1, 5, 5, 3, 0 };

    std::vector<int> v =  { 2, 4, 1, };
    for (auto a: aoc::range(1 << 12))
    {
        State state;
        state.a    = a;
        state.b    = 0;
        state.c    = 0;
        state.prog = prog;

        state.run();

        if ((v[0] == state.dump[0]) && (v[1] == state.dump[1]) && (v[2] == state.dump[2]))
        {
            cout << a << endl;
            auto octal = to_octal(a);
            print(octal);
        }
    } 
    return 0;
}


void run(const char* filename)
{
    State state;
    state.a = 44374556;
    state.b = 0;
    state.c = 0;
    state.prog = vector<int>{ 2, 4, 1, 5, 7, 5, 1, 6, 0, 3, 4, 1, 5, 5, 3, 0 };
    // 2, 4, com=a   bst   b = a % 8         // b = low 3 bits of a
    // 1, 5, lit     bxl   b = b ^ 5         // b XOR 5 - permute
    // 7, 5, com=b   cdv   c = a >> b        // c = higher bits of a
    // 1, 6, lit     bxl   b = b ^ 6         // b XOR 6 - permute
    // 0, 3, com=3   adv   a = a >> 3        // a >>= 3
    // 4, 1, n/a     bxc   b = b ^ c         // b XOR c - permute
    // 5, 5, com=b   out   b % 8
    // 3, 0          jnz   pc = 0        

    auto p1 = part1(state);
    std::cout << "Part1: " << p1 << '\n';   

    auto p2 = part2(state);
    std::cout << "Part2: " << p2 << '\n';
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
