#include "../include/JSONPrinter.h"
#include "../include/parser.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

// ---------------------
// Escaping the strings to be valid JSON
// ---------------------

// ------------------------------------------------------------
// WHY WE ESCAPE STRINGS IN JSONPRINTER
//
// JSONPrinter prints the JSON tree for debugging / pretty print.
// But JSONValues may contain REAL control chars like '\n' or '\t'
// because the tokenizer interprets escape sequences.
//
// Printing these raw to console produces:
//
//      Hello    World
//      He said "Yo!"
//
// which BREAKS the pretty-printed layout and looks like invalid JSON.
//
// Therefore JSONPrinter also re-escapes strings (same as serializer)
// so the printed output is readable and valid-looking.
//
// IMPORTANT: JSONPrinter is NOT serialization, but must still escape
// control characters to avoid confusing debug output.
// ------------------------------------------------------------

string JSONPrinter::escapeString(const std::string& s) {
    ostringstream out;

    for (char c : s) {
        switch (c) {
            case '\"': out << "\\\""; break;
            case '\\': out << "\\\\"; break;
            case '\b': out << "\\b";  break;
            case '\f': out << "\\f";  break;
            case '\n': out << "\\n";  break;
            case '\r': out << "\\r";  break;
            case '\t': out << "\\t";  break;

            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    out << "\\u"
                        << setw(4) << setfill('0') << uppercase << hex
                        << (int)c;
                } else {
                    out << c;
                }
        }
    }

    return out.str();
}

void JSONPrinter::printIndent(int indent) {
    for (int i = 0; i < indent; ++i) {
        cout << ' ';
    }
}

void JSONPrinter::print(const JSONValue& value, int indent) {

    // --- STRING ---
    if (holds_alternative<string>(value)) {
        cout << "\"" << escapeString(get<string>(value)) << "\"";
    }

    // --- NUMBER ---
    else if (holds_alternative<double>(value)) {
        cout << get<double>(value);
    }

    // --- BOOLEAN ---
    else if (holds_alternative<bool>(value)) {
        cout << (get<bool>(value) ? "true" : "false");
    }

    // --- NULL ---
    else if (holds_alternative<nullptr_t>(value)) {
        cout << "null";
    }

    // --- ARRAY ---
    else if (holds_alternative<JSONArray>(value)) {
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

    // --- OBJECT ---
    else if (holds_alternative<JSONObject>(value)) {
        const JSONObject& obj = get<JSONObject>(value);

        cout << "{\n";
        size_t count = 0;

        for (const auto& [key, valPtr] : obj) {
            printIndent(indent + 2);
            cout << "\"" << escapeString(key) << "\": ";
            print(*valPtr, indent + 2);

            if (count < obj.size() - 1) cout << ",";
            cout << "\n";
            ++count;
        }

        printIndent(indent);
        cout << "}";
    }
}


