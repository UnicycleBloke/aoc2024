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


bool can_grow_cluster(const set<int>& cluster, const int extra, map<int, vector<int>>& neighbours)
{
    // Every node in the cluster must be a neighbour of extra for exta to be able to extend the cluster.
    const auto& n = neighbours[extra];
    for (const auto& p: cluster)
    {
        if (find(n.begin(), n.end(), p) == n.end()) return false;
    }
    return true;
}


void grow_cluster(const set<int>& cluster, map<int, vector<int>>& neighbours, set<int>& consumed, set<int>& max_cluster)
{
    // Have found a larger subgraph?
    if (cluster.size() > max_cluster.size())
        max_cluster = cluster;

    // Set of potential extensions to the cluster.
    set<int> extra;
    for (const auto& p: cluster)
    {
        for (const auto& n: neighbours[p])
        {
            // We already used this node elsewhere.
            if (consumed.find(n) != consumed.end()) continue;
            // We already have this node in the cluster.
            if (cluster.find(n) != cluster.end()) continue; 
            extra.insert(n);
        }
    }

    // Try each extra node in turn and recurse if these form a cluster.
    bool can_grow = false;
    for (const auto& e: extra)
    {
        if (can_grow_cluster(cluster, e, neighbours))
        {
            can_grow = true;
            auto cluster2 = cluster;
            cluster2.insert(e);
            grow_cluster(cluster2, neighbours, consumed, max_cluster);
        }
    }

    // We have found a maximal subgraph. Mark all the nodes as consumed.
    if (!can_grow) 
    {
        for (const auto& c: cluster) 
            consumed.insert(c);
    }
}


int to_int(const string& p)
{
    return (p[0] - 'a') * 256 + (p[1] - 'a');
}


void print(int p)
{
    char c1 = 'a' + (p / 256);
    char c2 = 'a' + (p % 256);
    cout << c1 << c2 << ",";
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    // Build the set of immediate neighbours for each node.
    map<int, vector<int>> neighbours;
    for (auto [p1, p2]: input)
    {        
        int i1 = to_int(p1);
        int i2 = to_int(p2);
        neighbours[i1].push_back(i2);
        neighbours[i2].push_back(i1);
    }

    set<int> max_cluster;
    
    // Use a greedy algorithm to grow a fully connnected (aka complete) subgraph from each node. 
    // Each node can be in multiple complete subgraphs but only one maximal complete subgraph. A
    // maximal subgraph is one which cannot be further extended. This means that once we find a 
    // maximal subgraph containing P, we can regard P as unavailable for other subgraphs - consumed.
    set<int> consumed;
    for (const auto& [p, v]: neighbours)     
    {
        print(p);
        cout << endl;
        if (consumed.find(p) != consumed.end()) continue;

        // Seed cluster 
        set<int> cluster;
        cluster.insert(p);
        grow_cluster(cluster, neighbours, consumed, max_cluster);
    }

    for (const auto& p: max_cluster)
        print(p);
        //cout << p << ","; 
    cout << endl;

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
