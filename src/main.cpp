#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "../include/FileUtils.h"
#include "../include/token.h"
#include "../include/tokenizer.h"
#include "../include/parser.h"
#include "../include/JSONSerializer.h"
#include "../include/JSONPrinter.h"
#include "../include/JSONPath.h"
#include "../include/JSONNavigator.h"

// ===============================================================
// Print CLI Usage
// ===============================================================
void printUsage() {
    std::cout << "Usage:\n";
    std::cout << "  json pretty <input.json>\n";
    std::cout << "  json minify <input.json>\n";
    std::cout << "  json validate <input.json>\n";
    std::cout << "  json show <input.json>\n";
    std::cout << "  json get <input.json> <path>\n";
    std::cout << "  json set <input.json> <path> <value>\n";
}

// ===============================================================
// Auto-detect JSON value type for "set"
// ===============================================================
JSONValue parseValueFromString(const std::string& s) {
    if (s == "true")  return JSONValue(true);
    if (s == "false") return JSONValue(false);
    if (s == "null")  return JSONValue(nullptr);

    bool isNum = true;
    bool hasDot = false;

    for (char c : s) {
        if (c == '.') hasDot = true;
        else if (!isdigit(c) && c != '-') isNum = false;
    }

    if (isNum) {
        try {
            double v = std::stod(s);
            return JSONValue(v);
        } catch (...) {}
    }

    // string with quotes → remove quotes
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
        return JSONValue(s.substr(1, s.size() - 2));

    return JSONValue(s);
}

// ===============================================================
// Print a line + caret for error reporting
// ===============================================================
void printErrorContext(const std::string& source, int line, int column) {
    std::istringstream ss(source);
    std::string currentLine;
    int current = 1;

    while (std::getline(ss, currentLine)) {
        if (current == line) {
            std::cout << currentLine << "\n";
            for (int i = 1; i < column; i++) std::cout << " ";
            std::cout << "^\n";
            return;
        }
        current++;
    }
}

// ===============================================================
// MAIN
// ===============================================================
int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cerr << "❌ Error: Not enough arguments.\n\n";
        printUsage();
        return 1;
    }

    std::string command = argv[1];
    std::string inputPath = argv[2];

    try {
        // ----------- Read JSON File -----------
        std::string inputJSON = FileUtils::readFile(inputPath);

        // ----------- Tokenize -----------
        Tokenizer tokenizer(inputJSON);
        std::vector<Token> tokens = tokenizer.tokenize();

        // ----------- Parse -----------
        Parser parser(tokens);
        JSONValue root = parser.parse();

        // ===============================================================
        // 1) PRETTY
        // ===============================================================
        if (command == "pretty") {
            std::string pretty = JSONSerializer::serialize(root);

            std::string outputPath =
                inputPath.substr(0, inputPath.find_last_of('.')) + "_pretty.json";

            FileUtils::writeFile(outputPath, pretty);
            std::cout << "✔ Pretty JSON written to: " << outputPath << "\n";
            return 0;
        }

        // ===============================================================
        // 2) MINIFY
        // ===============================================================
        if (command == "minify") {
            std::string compact = JSONSerializer::serializeCompact(root);

            std::string outputPath =
                inputPath.substr(0, inputPath.find_last_of('.')) + "_minified.json";

            FileUtils::writeFile(outputPath, compact);
            std::cout << "✔ Minified JSON written to: " << outputPath << "\n";
            return 0;
        }

        // ===============================================================
        // 3) VALIDATE
        // ===============================================================
        if (command == "validate") {
            std::cout << "✔ Valid JSON\n";
            return 0;
        }

        // ===============================================================
        // 4) SHOW
        // ===============================================================
        if (command == "show") {
            JSONPrinter::print(root, 0);
            std::cout << "\n";
            return 0;
        }

        // ===============================================================
        // 5) GET
        // ===============================================================
        if (command == "get") {
            if (argc < 4) {
                std::cerr << "❌ Missing path.\n";
                return 1;
            }

            std::string path = argv[3];
            auto parsedPath = JSONPath::parse(path);
            JSONValue& result = JSONNavigator::get(root, parsedPath);

            JSONPrinter::print(result);
            std::cout << "\n";

            return 0;
        }

        // ===============================================================
        // 6) SET
        // ===============================================================
        if (command == "set") {
            if (argc < 5) {
                std::cerr << "❌ Missing path or value.\n";
                return 1;
            }

            std::string path = argv[3];
            std::string newValueStr = argv[4];

            auto parsedPath = JSONPath::parse(path);
            JSONValue& target = JSONNavigator::get(root, parsedPath);

            // auto-detect type
            target = parseValueFromString(newValueStr);

            std::string updated = JSONSerializer::serialize(root);
            FileUtils::writeFile(inputPath, updated);

            std::cout << "✔ Updated value and saved to " << inputPath << "\n";
            return 0;
        }

        // ===============================================================
        // UNKNOWN COMMAND
        // ===============================================================
        std::cerr << "❌ Unknown command: " << command << "\n\n";
        printUsage();
        return 1;
    }

    // ===============================================================
    // ERROR HANDLING
    // ===============================================================

    // --- Parser Errors with (line, column) ---
    catch (const JSONParseError& e) {
        std::cerr << "\n❌ JSON Parse Error at line "
                  << e.line << ", column " << e.column << ":\n";
        std::cerr << "   " << e.what() << "\n\n";

        try {
            std::string inputJSON = FileUtils::readFile(inputPath);
            printErrorContext(inputJSON, e.line, e.column);
        } catch (...) {}

        return 1;
    }

    // --- Tokenizer / other runtime errors ---
    catch (const std::runtime_error& e) {
        std::cerr << "\n❌ Error while tokenizing/parsing:\n";
        std::cerr << "   " << e.what() << "\n";
        return 1;
    }

    // --- Unknown exceptions ---
    catch (const std::exception& e) {
        std::cerr << "\n❌ Internal Error: " << e.what() << "\n";
        return 1;
    }
}






/*
===============================================================
STRING ESCAPING — WHY THIS IS CRITICAL FOR A JSON PARSER
===============================================================

JSON allows many characters to appear INSIDE strings using escape
sequences like:

    \"   \\   \n   \t   \b   \r   \f   \/  

When parsing JSON, the tokenizer must:
    - recognize these escape sequences
    - interpret them into REAL characters in memory 
      (e.g., '\n' instead of backslash+n)

When serializing JSON, the serializer must:
    - convert those real characters BACK into escape sequences 
      so that the output JSON is valid text

When pretty-printing JSON, the printer must:
    - escape strings again so multi-line strings don't break layout

If any of these steps are skipped or done incorrectly, the JSON
parser will either:
    - fail to parse valid JSON
    - produce invalid JSON on output
    - display confusing output during debugging
This implementation carefully handles string escaping in all three
*/





