#pragma once
#include "parser.h"
#include <iostream>
#include <string>

class JSONPrinter {
public:
    static void print(const JSONValue& value, int indent = 0);

private:
    static void printIndent(int indent);
    static std::string escapeString(const std::string& s);
};
