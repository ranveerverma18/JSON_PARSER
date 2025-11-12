#include <bits/stdc++.h>
#include "../include/token.h"
#include "../include/tokenizer.h"
#include "../include/parser.h"

using namespace std;

// Aliases from parser.h — ensure they exist there too
using JSONObject = std::unordered_map<std::string, std::shared_ptr<JSONValue>>;
using JSONArray  = std::vector<std::shared_ptr<JSONValue>>;

void printJSON(const JSONValue &v, int indent = 0);

void indentPrint(int n) {
    for (int i = 0; i < n; ++i) cout << ' ';
}

void printObject(const JSONObject &obj, int indent) {
    cout << "{\n";
    bool first = true;
    for (const auto &[key, valPtr] : obj) {
        if (!first) cout << ",\n";
        first = false;
        indentPrint(indent + 2);
        cout << '"' << key << "\": ";
        printJSON(*valPtr, indent + 2); // dereference shared_ptr
    }
    cout << "\n";
    indentPrint(indent);
    cout << "}";
}

void printArray(const JSONArray &arr, int indent) {
    cout << "[\n";
    bool first = true;
    for (const auto &elPtr : arr) {
        if (!first) cout << ",\n";
        first = false;
        indentPrint(indent + 2);
        printJSON(*elPtr, indent + 2);
    }
    cout << "\n";
    indentPrint(indent);
    cout << "]";
}

void printJSON(const JSONValue &v, int indent) {
    if (holds_alternative<string>(v)) {
        cout << '"' << get<string>(v) << '"';
    } else if (holds_alternative<double>(v)) {
        cout << get<double>(v);
    } else if (holds_alternative<bool>(v)) {
        cout << (get<bool>(v) ? "true" : "false");
    } else if (holds_alternative<nullptr_t>(v)) {
        cout << "null";
    } else if (holds_alternative<JSONArray>(v)) {
        printArray(get<JSONArray>(v), indent);
    } else if (holds_alternative<JSONObject>(v)) {
        printObject(get<JSONObject>(v), indent);
    } else {
        cout << "UNKNOWN";
    }
}

int main() {
    string json = R"({
        "name": "Ranveer",
        "age": 20,
        "student": true,
        "nickname": null,
        "height": -5.8,
        "marks": [98, 95, 93],
        "profile": {
            "college": "SRCC",
            "year": 3
        }
    })";

    try {
        Tokenizer tokenizer(json);
        vector<Token> tokens = tokenizer.tokenize();

        Parser parser(tokens);
        JSONValue root = parser.parse();

        printJSON(root);
        cout << "\n";
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}




