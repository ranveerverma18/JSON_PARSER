#include "../include/JSONNavigator.h"
#include <stdexcept>

JSONValue& JSONNavigator::get(JSONValue& root, const std::vector<PathElement>& path) {
    JSONValue* current = &root;

    for (const auto& p : path) {
        if (p.isIndex) {
            // must be array
            if (!current->isArray())
                throw std::runtime_error("Expected array in path");

            JSONArray& arr = current->asArray();
            if (p.index < 0 || p.index >= (int)arr.size())
                throw std::runtime_error("Array index out of bounds");

            current = arr[p.index].get();
        } 
        else {
            // must be object
            if (!current->isObject())
                throw std::runtime_error("Expected object in path");

            JSONObject& obj = current->asObject();
            if (!obj.count(p.key))
                throw std::runtime_error("Key not found: " + p.key);

            current = obj[p.key].get();
        }
    }

    return *current;
}
