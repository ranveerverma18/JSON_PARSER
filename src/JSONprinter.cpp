#include "../include/JSONPrinter.h"
using namespace std;

void JSONPrinter::printIndent(int indent) {
    for (int i = 0; i < indent; ++i) {
        cout << ' ';
    }
}

void JSONPrinter::print(const JSONValue& value, int indent) {
    if (holds_alternative<string>(value)) {
        cout << '"' << get<string>(value) << '"';
    } 
    else if (holds_alternative<double>(value)) {
        cout << get<double>(value);
    } 
    else if (holds_alternative<bool>(value)) {
        cout << (get<bool>(value) ? "true" : "false");
    } 
    else if (holds_alternative<nullptr_t>(value)) {
        cout << "null";
    } 
    else if (holds_alternative<JSONArray>(value)) {    //for handling the arrays
        const JSONArray& arr = get<JSONArray>(value);
        cout << "[\n";
        for (size_t i = 0; i < arr.size(); ++i) {
            printIndent(indent + 2);
            print(*arr[i], indent + 2);
            if (i < arr.size() - 1) cout << ",";
            cout << "\n";
        }
        printIndent(indent);
        cout << "]";
    } 
    else if (holds_alternative<JSONObject>(value)) {  //for handling the objects
        const JSONObject& obj = get<JSONObject>(value);
        cout << "{\n";
        size_t count = 0;
        for (const auto& [key, valPtr] : obj) {
            printIndent(indent + 2);
            cout << '"' << key << "\": ";
            print(*valPtr, indent + 2);
            if (count < obj.size() - 1) cout << ",";
            cout << "\n";
            ++count;
        }
        printIndent(indent);
        cout << "}";
    }
}

