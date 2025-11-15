#pragma once
#include <string>
#include <vector>

struct PathElement {
    bool isIndex;      // true = array index, false = object key
    std::string key;   // if object key
    int index;         // if array index
};

class JSONPath {
public:
    static std::vector<PathElement> parse(const std::string& path);
};
