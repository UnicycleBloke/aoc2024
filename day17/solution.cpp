#include "utils.h"


struct State
{
    uint64_t a{};
    uint64_t b{};
    uint64_t c{};
    vector<uint64_t> prog;

    uint64_t pc{};
    vector<uint64_t> dump;

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
            // adv - division
            case 0: a = a / (1UL << combo); break;
            // bxl 
            case 1: b = b ^ literal; break;
            // bst
            case 2: b = combo % 8UL; break;
            // jnz
            case 3: if (a != 0UL) pc = literal; break;
            // bxc
            case 4: b = b ^ c; break;
            // out
            case 5: dump.push_back(combo % 8); break;
            // bdv
            case 6: b = a / (1UL << combo); break;
            // cdv
            case 7: c = a / (1UL << combo); break;
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


auto part2(vector<uint64_t>& prog)
{
    aoc::timer timer;

    uint64_t digits = 1;
    uint64_t shift  = 1 << ((digits + 3U) * 3U);
    set<uint64_t> as;
    for (auto a: aoc::range(shift))
        as.insert(a);

    while (digits <= prog.size())
    {
        set<uint64_t> as2;

        for (auto a: as)
        {
            State state;
            state.a    = a;
            state.b    = 0;
            state.c    = 0;
            state.prog = prog;
            state.run();

            int matches = 0;
            for (auto d: aoc::range(digits))
                matches += (prog[d] == state.dump[d]);

            if (matches >= digits)
            {
                for (uint64_t x: aoc::range(8UL))
                {
                    uint64_t b = x * shift;
                    as2.insert(a | b);
                }
            }

            if (matches == prog.size())
                return a;
        }

        digits +=  1;
        shift  <<= 3;

        as = as2; 
    }

    return 0UL;
}


void run(const char* filename)
{
    State state;
    state.a = 44374556;
    state.b = 0;
    state.c = 0;
    state.prog = vector<uint64_t>{ 2, 4, 1, 5, 7, 5, 1, 6, 0, 3, 4, 1, 5, 5, 3, 0 };
    // 2, 4, com=a   bst   b = a % 8         // b = low 3 bits of a
    // 1, 5, lit     bxl   b = b ^ 5         // b XOR 5 - permute
    // 7, 5, com=b   cdv   c = a >> b        // c = higher bits of a
    // 1, 6, lit     bxl   b = b ^ 6         // b XOR 6 - permute
    // 0, 3, com=3   adv   a = a >> 3        // a >>= 3
    // 4, 1, n/a     bxc   b = b ^ c         // b XOR c - permute
    // 5, 5, com=b   out   b % 8
    // 3, 0          jnz   pc = 0        

    auto p1 = part1(state);
    cout << "Part1: " << p1 << '\n';   

    auto p2 = part2(state.prog);
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
    catch (exception& e)
    {
        cout << e.what();
    }
}
