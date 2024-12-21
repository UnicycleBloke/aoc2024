#include "utils.h"

struct KeyBoard
{
    using Point = pair<int, int>;
    map<char, Point> keys;
    map<Point, char> keys2;
    int row;
    int col;
    int prow;
    int pcol;

    KeyBoard(int rows, int cols, const char* data)
    {
        for (auto r: aoc::range(rows))
        {
            for (auto c: aoc::range(cols))
            {
                char key = data[cols * r + c];
                keys[key] = {r, c};
                keys2[{r, c}] = key;
                if (key == 'A')
                {
                    row = r;
                    col = c;
                }
            }
        }
        prow = row;
        pcol = col;
    }

    string enter(const string& sequence)
    {
        ostringstream os;
        for (auto c: sequence)
            os << press(c);
        return os.str();
    }

    string panic()
    {
        return (keys2[{row, col}] == '*') ? " panic" : "";
    }

    void dump(char key)
    {
        // cout << key << ": (" << prow << ", " << pcol << ") " << keys2[{prow, pcol}]; 
        // cout << " -> (" << row << ", " << col << ") " << keys2[{row, col}] << " " << panic() << endl;
        // prow = row;
        // pcol = col;
    }

    string press(char key)
    {
        ostringstream os;
        auto [row2, col2] = keys[key];

        auto move_horz = [&]()
        {
            while (col2 != col)
            {
                auto dir = aoc::sgn(col2 - col);
                switch (dir)
                {
                    case +1: os << '>'; break;
                    case -1: os << '<'; break;
                }
                col += dir;
                dump(key);
            }
        };

        auto move_vert = [&]()
        {
            while (row2 != row)
            {
                auto dir = aoc::sgn(row2 - row);
                switch (dir)
                {
                    case +1: os << 'v'; break;
                    case -1: os << '^'; break;
                }
                row += dir;
                dump(key);
            }
        };

        // if (col == col2)
        // {
        //     move_vert();
        // }
        // else if (row == row2)
        // {
        //     move_horz();
        // }
        // else if (keys2[{row, col2}] == '*')
        if (keys2[{row, col2}] == '*')
        {
            // Move to {row2, col} first
            move_vert();
            move_horz();
        }
        else
        {
            move_horz();
            move_vert();
        }

        //cout << endl;
        os << 'A';
        return os.str();
    }
};


const KeyBoard keypad{4, 3, "789456123*0A" };
const KeyBoard dirpad{2, 3, "*^A<v>" };


int number(const string& code)
{
    int result = 0;
    for (char c: code)
    {
        if (c == 'A') continue;
        result = result * 10 + (c - '0');
    }
    return result;
}


void analyse(const string& sequence)
{
    map<char, int> counts;
    for (char c: sequence)
        counts[c] = counts[c] + 1;

    cout << sequence << endl;
    for (auto [c, n]: counts)
        cout << c << " " << n << endl;
}


KeyBoard pad{4, 3, "789456123*0A" };
KeyBoard dir1{2, 3, "*^A<v>" };
KeyBoard dir2{2, 3, "*^A<v>" };
KeyBoard dir3{2, 3, "*^A<v>" };


int calculate(const string& code)
{
    // auto pad  = keypad;
    // auto dir1 = dirpad;
    // auto dir2 = dirpad;

    cout << code << ": " << code.size() << ": " << code << endl;
    auto seq1 = pad.enter(code);   
    cout << endl;

    cout << code << ": " << seq1.size() << ": " << seq1 << endl;
    auto seq2 = dir1.enter(seq1);
    cout << endl;

    cout << code << ": " << seq2.size() << ": " << seq2 << endl;
    auto seq3 = dir2.enter(seq2);
    cout << endl;

    cout << code << ": " << seq3.size() << ": " << seq3 << endl;

    //analyse(seq3);
    cout << seq3.size() << " * "<< number(code) << endl;
    return number(code) * seq3.size();
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int result = 0;
    for (auto code: input)
        result += calculate(code);

    // analyse("<vA<AA>>^AvAA<^A>A<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A");
    // analyse("<v<A>>^AAAvA^A<vA<AA>>^AvAA<^A>A<v<A>A>^AAAvA<^A>A<vA>^A<A>A");
    // analyse("<v<A>>^A<vA<A>>^AAvAA<^A>A<v<A>>^AAvA^A<vA>^AA<A>A<v<A>A>^AAAvA<^A>A");
    // analyse("<v<A>>^AA<vA<A>>^AAvAA<^A>A<vA>^A<A>A<vA>^A<A>A<v<A>A>^AAvA<^A>A");
    // analyse("<v<A>>^AvA^A<vA<AA>>^AAvA<^A>AAvA^A<vA>^AA<A>A<v<A>A>^AAAvA<^A>A");

    return result;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;
    return 0;
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
