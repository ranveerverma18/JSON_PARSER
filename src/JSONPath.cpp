#include "../include/JSONPath.h"
#include<bits/stdc++.h>

std::vector<PathElement> JSONPath::parse(const std::string& path) {
    std::vector<PathElement> result;
    int n = path.size();
    int i = 0;

    while (i < n) {
        // ----- Parse key: a sequence of letters/numbers/underscores
        std::string key;
        while (i < n && (isalnum(path[i]) || path[i] == '_')) {
            key += path[i++];
        }

        if (!key.empty()) {
            result.push_back({false, key, -1});
        }

        // ----- Parse array index: [number]
        if (i < n && path[i] == '[') {
            i++; // skip '['
            std::string num;
            while (i < n && isdigit(path[i])) {
                num += path[i++];
            }
            if (i >= n || path[i] != ']')
                throw std::runtime_error("Invalid array index in path: " + path);
            i++; // skip ']'

            result.push_back({true, "", std::stoi(num)});
        }

        // ----- Skip dot
        if (i < n && path[i] == '.')
            i++;
    }

    return result;
}
