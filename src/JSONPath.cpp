#include "../include/JSONPath.h"
#include <stdexcept>
#include <cctype>

std::vector<PathElement> JSONPath::parse(const std::string& path) {
    std::vector<PathElement> result;
    int n = path.size();
    int i = 0;

    while (i < n) {
        // ----- Parse key: a sequence of letters/numbers/underscores
        std::string key;
        while (i < n && (std::isalnum(path[i]) || path[i] == '_')) {
            key += path[i++];
        }

        if (!key.empty()) {
            result.push_back({false, key, -1});
        }

        // ----- Parse array index: [number]
        if (i < n && path[i] == '[') {
            i++; // skip '['
            std::string num;
            while (i < n && std::isdigit(path[i])) {
                num += path[i++];
            }
            if (i >= n || path[i] != ']')
                throw std::runtime_error("Invalid array index in path: " + path);
            i++; // skip ']'

            if (num.empty())
                throw std::runtime_error("Empty array index in path: " + path);

            result.push_back({true, "", std::stoi(num)});
        }

        // ----- Skip dot
        if (i < n && path[i] == '.') {
            i++;
            // Validate that dot is not trailing or consecutive
            if (i >= n || path[i] == '.')
                throw std::runtime_error("Invalid path syntax (trailing or consecutive dots): " + path);
        }
    }

    if (result.empty())
        throw std::runtime_error("Empty path provided");

    return result;
}
