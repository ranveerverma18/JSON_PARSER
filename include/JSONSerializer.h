#pragma once
#include <string>
#include "parser.h"   

class JSONSerializer {
public:
    static std::string serialize(const JSONValue& value, int indent = 0);
    static std::string serialize(const std::shared_ptr<JSONValue>& ptr, int indent = 0);

private:
    static std::string indentString(int indent);
};

