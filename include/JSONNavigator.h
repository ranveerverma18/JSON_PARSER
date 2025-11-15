#pragma once
#include "parser.h"
#include "JSONPath.h"

class JSONNavigator {
public:
    static JSONValue& get(JSONValue& root, const std::vector<PathElement>& path);
};
