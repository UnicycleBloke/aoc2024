#include "utils.h"


struct State
{
    int64_t a{};
    int64_t b{};
    int64_t c{};
    vector<int> prog;

    int64_t pc{};
    vector<int> dump;

    void run()
    {
        while (pc < prog.size())
            process();
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


template <typename T>
auto part2(T input)
{
    aoc::timer timer;

    int a;
    while (true)
    {
        State state;
        state.a = a;
        state.b = 0;
        state.c = 0;
        state.prog = vector<int>{ 0, 3, 5, 4, 3, 0 };
        //0, 3,  a = a / (1 << 3)    a = a >> 3
        //5, 4,  out << (a & 7)
        //3, 0   jnz 0
        // while (a)
        // {
        //     a = a >> 3;
        //     dump << (a & 7);
        // } 

        state.run();
        if (state.prog == state.dump) break;
        //cout << a << endl;
        ++a;
    }

    return a;
}


void run(const char* filename)
{
    State state;
    state.a = 44374556;
    state.b = 0;
    state.c = 0;
    state.prog = vector<int>{ 2, 4, 1, 5, 7, 5, 1, 6, 0, 3, 4, 1, 5, 5, 3, 0 };
    // 2, 4,   b = a & 7
    // 1, 5,   b = b ^ 5
    // 7, 5,   c = a >> b
    // 1, 6,   b = b ^ 6
    // 0, 3,   a = a >> 3
    // 4, 1,   b = b ^ c;
    // 5, 5,   out (b & 7)
    // 3, 0    jnz 0
    // while (a)
    // {
    //     b = a & 7
    //     b = b ^ 5
    //     c = a >> b
    //     b = b ^ 6
    //     a = a >> 3
    //     b = b ^ c;
    //     out (b & 7)
    // }

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
