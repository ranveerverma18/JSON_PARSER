#include "../include/JSONPrinter.h"
#include "../include/JSONUtils.h"
#include "../include/parser.h"
#include <iostream>

void JSONPrinter::printIndent(int indent) {
    for (int i = 0; i < indent; ++i) {
        std::cout << ' ';
    }
}

void JSONPrinter::print(const JSONValue& value, int indent) {

    // --- STRING ---
    if (std::holds_alternative<std::string>(value)) {
        std::cout << "\"" << JSONUtils::escapeString(std::get<std::string>(value)) << "\"";
    }

    // --- NUMBER ---
    else if (std::holds_alternative<double>(value)) {
        std::cout << JSONUtils::formatNumber(std::get<double>(value));
    }

    // --- BOOLEAN ---
    else if (std::holds_alternative<bool>(value)) {
        std::cout << (std::get<bool>(value) ? "true" : "false");
    }

    // --- NULL ---
    else if (std::holds_alternative<std::nullptr_t>(value)) {
        std::cout << "null";
    }

    // --- ARRAY ---
    else if (std::holds_alternative<JSONArray>(value)) {
        const JSONArray& arr = std::get<JSONArray>(value);

        std::cout << "[\n";
        for (size_t i = 0; i < arr.size(); ++i) {
            printIndent(indent + 2);
            print(*arr[i], indent + 2);
            if (i < arr.size() - 1) std::cout << ",";
            std::cout << "\n";
        }
        printIndent(indent);
        std::cout << "]";
    }

    // --- OBJECT ---
    else if (std::holds_alternative<JSONObject>(value)) {
        const JSONObject& obj = std::get<JSONObject>(value);

        std::cout << "{\n";
        size_t count = 0;

        for (const auto& [key, valPtr] : obj) {
            printIndent(indent + 2);
            std::cout << "\"" << JSONUtils::escapeString(key) << "\": ";
            print(*valPtr, indent + 2);

            if (count < obj.size() - 1) std::cout << ",";
            std::cout << "\n";
            ++count;
        }

        printIndent(indent);
        std::cout << "}";
    }
}


