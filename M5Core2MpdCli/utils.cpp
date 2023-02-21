#include "utils.h"

vector<string> split(const string& s, char delim)
{
    vector<string> result;
    size_t start;
    size_t end = 0;

    while ((start = s.find_first_not_of(delim, end)) != std::string::npos) {
        end = s.find(delim, start);
        result.push_back(s.substr(start, end - start));
    }
    return result;
}
