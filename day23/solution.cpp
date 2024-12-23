#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    map<string, vector<string>> neighbours;
    for (auto [p1, p2]: input)
    {        
        neighbours[p1].push_back(p2);
        neighbours[p2].push_back(p1);
    }

    set<set<string>> triples;
    for (const auto& [p1, v1]: neighbours)     
    {
        if (p1[0] == 't')
        {
            auto s1 = v1.size();
            for (auto i: aoc::range(s1))
            {
                const auto& p2 = v1[i];
                const auto& v2 = neighbours[p2];
                for (auto j: aoc::range(i, s1))
                {
                    const auto& p3 = v1[j];
                    if (find(v2.begin(), v2.end(), p3) != v2.end())
                    {
                        triples.insert({p1, p2, p3});
                    }
                }
            }
        }
    }

    return triples.size();
}


bool is_cluster(const set<string>& cluster, const string& extra, map<string, vector<string>>& neighbours)
{
    const auto& v = neighbours[extra];
    for (const auto& p: cluster)
    {
        if (find(v.begin(), v.end(), p) == v.end()) return false;
    }

    return true;
}

set<string> max_cluster;

void find_cluster(const set<string>& cluster, map<string, vector<string>>& neighbours)
{
    cout << cluster.size() << endl;

    if (cluster.size() > max_cluster.size())
        max_cluster = cluster;

    // Set of potential additions.
    set<string> extra;
    for (const auto& p: cluster)
    {
        for (const auto& p2: neighbours[p])
        {
            if (cluster.find(p2) == cluster.end()) extra.insert(p2);
        }
    }

    for (const auto& e: extra)
    {
        if (is_cluster(cluster, e, neighbours))
        {
            auto cluster2 = cluster;
            cluster2.insert(e);
            find_cluster(cluster2, neighbours);
        }
    }
}


template <typename T>
auto part2(T& input)
{
    map<string, vector<string>> neighbours;
    for (auto [p1, p2]: input)
    {        
        neighbours[p1].push_back(p2);
        neighbours[p2].push_back(p1);
    }

    for (const auto& [p, v]: neighbours)     
    {
        set<string> cluster;
        cluster.insert(p);
        for (const auto& n: v) cluster.insert(n);

        bool is_cluster = true;
        for (const auto& n: v) 
        {
            const auto& v2 = neighbours[n];
            for (const auto& n2: v2) 
            {
                if (n2 == n) continue;
                is_cluster &= (find(v2.begin(), v2.end(), n2) != v2.end());
            }
        }

        static int count{};
        if (is_cluster)
        {
            cout << count++ << cluster.size() << ": ";
            for (const auto& p: cluster) cout << p << ","; cout << endl;
        }
    }

    return max_cluster.size();
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<string, string>(filename, R"((\w+)-(\w+))");

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
