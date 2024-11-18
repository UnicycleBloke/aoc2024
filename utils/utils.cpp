#include "utils.h"
#include <sstream>


namespace aoc {


void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        [](unsigned char ch) { return !std::isspace(ch); }));
}


void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
        [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}


void trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}


std::string replace(std::string source, const std::string& search, const std::string& replace)
{
    size_t pos = 0;
    while ((pos = source.find(search, pos)) != std::string::npos)
    {
        source.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return source;
}


std::vector<std::string> split(std::string source, std::string delim, Blanks allow_blanks, Trim trim_subs)
{
    std::vector<std::string> result;

    size_t start = 0;
    size_t end = source.find(delim);
    while (end != std::string::npos)
    {
        auto sub = source.substr(start, end - start);
        if (trim_subs == Trim::Yes)
            trim(sub);
        if (allow_blanks == Blanks::Allow || (sub.size() > 0))
            result.push_back(sub);

        start = end + delim.length();
        end   = source.find(delim, start);
    }

    auto sub = source.substr(start, end - start);
    if (trim_subs == Trim::Yes)
        trim(sub);
    if (allow_blanks == Blanks::Allow || (sub.size() > 0))
        result.push_back(sub);

    return result;
}


std::vector<std::string> read_lines(std::istream& is, Blanks allow_blanks, Trim trim_lines)
{
    std::vector<std::string> lines;

    while (!is.eof())
    {
        std::string line;
        std::getline(is, line);

        if (trim_lines == Trim::Yes)
            trim(line);
        if (allow_blanks == Blanks::Allow || (line.size() > 0))
            lines.push_back(line);
    }

    return lines;
}


std::vector<std::string> read_lines(std::string filename, Blanks allow_blanks, Trim trim_lines)
{
    std::ifstream is{filename};
    return read_lines(is, allow_blanks, trim_lines);
}


std::vector<std::vector<std::string>> read_groups(std::istream& is)
{
    std::ostringstream ss;
    std::vector<std::vector<std::string>> groups;

    std::vector<std::string> lines = read_lines(is, Blanks::Allow);
    std::vector<std::string> group;

    for (const auto& line: lines)
    {
        if (line.size() > 0)
        {
            group.push_back(line);
        }
        else
        {
            if (group.size() > 0)
                groups.push_back(group);
            group.clear();
        }        
    }

    if (group.size() > 0)
        groups.push_back(group);
    return groups;
}


std::vector<std::vector<std::string>> read_groups(std::string filename)
{
    std::ifstream is{filename};
    return read_groups(is);
}


vector<string> make_grid_with_border(const vector<string>& input, size_t width, char value)
{
    string extra(width, value);
    string blank(input[0].size() + 2*width, value);

    vector<string> grid;
    for (auto i: range(width))
        grid.push_back(blank);     
    for (const auto& line: input)
        grid.push_back(extra + line + extra);
    for (auto i: range(width))
        grid.push_back(blank);  
    return grid;
}


vector<string> read_grid_with_border(std::string filename, size_t width, char value)
{
    auto lines = read_lines(filename);
    return make_grid_with_border(lines, width, value);
}


} // namespace aoc {
