#include "utils.h"


struct State
{
    int64_t a{};
    int64_t b{};
    int64_t c{};
    vector<int> prog;

    int64_t pc{};
    array<int, 20> dump{};
    int64_t i{};

    bool run(int64_t start)
    {
        a  = start;
        b  = 0;
        c  = 0;
        pc = 0; 
        i  = 0;    

        while (true)
        {
            auto result = process();
            if (result != 1) return result == 0;               
        }
    }

    int process()
    {
        if (pc >= prog.size()) return 0;
        auto opcode = prog[pc++];
        
        if (pc >= prog.size()) return 0;
        auto literal = prog[pc++];

        if (i >= prog.size()) return 2;

        auto combo = literal;
        switch (literal)
        {
            case 0: combo = 0; break;
            case 1: combo = 1; break;
            case 2: combo = 2; break;
            case 3: combo = 3; break;
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
                {
                    pc = literal;
                    // cout << "pc = " << pc << endl;
                    // cout << "  a = " << a << endl;
                    // cout << "  b = " << b << endl;
                    // cout << "  c = " << c << endl;
                }
                break;

            case 4: // bxc
                b = b ^ c;
                //cout << "b = b ^ c = " << b << endl;
                break;

            case 5: // out
                //dump.push_back(combo % 8);
                dump[i] = (combo % 8);
                if (dump[i] != prog[i]) return 2;
                ++i;
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

        return 1;
    }
};


template <typename T>
auto part1(T input)
{
    aoc::timer timer;

    // while (input.process());
    // for (auto c: input.dump)
    //     cout << c << ",";
    // cout << endl;

    // ostringstream os;
    // for (auto i: input.prog)
    //     os << i << ',';
    // auto prog = os.str();

    int a = 0;
    while (true) 
    {
        cout << "Trying a=" << a << endl;
        if (input.run(a) == 1) break;
        ++a;
    }

    // int64_t a = 44374556;
    // int64_t b = 0;
    // int64_t c = 0;
    // while (a != 0)
    // {
    //     b = a % 8;  // [0 1 2 3 4 5 6 7]
    //     b = b ^ 5;  // [5 4 7 6 1 0 3 2] Shuffle
    //     c = a / (1 << b);
    //     b = b ^ 6; 
    //     a = a / (1 << 3);
    //     b = b ^ c; 
    //     cout << (b % 8) << ",";           
    // }
    // cout << endl;

    return a;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;
    return 0;
}


void run(const char* filename)
{
    // auto regs  = aoc::read_lines<int>(filename, R"(Register \w: (\d+))");
    // auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 

    // State state;
    // state.a    = get<0>(regs[0]);
    // state.b    = get<0>(regs[1]);
    // state.c    = get<0>(regs[2]);
    // state.prog = aoc::make_vector<int>(lines[3]);

    // state.print();

    // Example
    State state;
    state.a = 2024;
    state.b = 0;
    state.c = 0;
    state.prog = vector<int>{ 0, 3, 5, 4, 3, 0 };

    // Y State state{10, 0, 0, { 5, 0, 5, 1, 5, 4 }};
    //State state{2024, 0, 0, { 0, 1, 5, 4, 3, 0 }};
    
    //Y State state{0, 0, 9, { 2, 6 }};
    //Y State state{0, 29, 0, { 1, 7 }}; 
    //Y State state{0, 2024, 43690, { 4, 0 }}; 

    // State state;
    // state.a = 44374556;
    // state.b = 0;
    // state.c = 0;
    // state.prog = vector<int>{ 2, 4, 1, 5, 7, 5, 1, 6, 0, 3, 4, 1, 5, 5, 3, 0 };



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
