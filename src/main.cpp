#include <iostream>
#include "../include/tokenizer.h"
#include "../include/parser.h"
#include "../include/JSONPrinter.h"

int main() {
    std::string input = R"({
        "name": "Ranveer",
        "age": 20,
        "student": true,
        "height": -5.8,
        "nickname": null,
        "marks": [98, 95, 93],
        "profile": {
            "year": 3,
            "college": "SRCC"
        }
    })";

    try {
        Tokenizer tokenizer(input);
        std::vector<Token> tokens = tokenizer.tokenize();

        Parser parser(tokens);
        JSONValue root = parser.parse();

        std::cout << "\n Original JSON:\n";
        JSONPrinter::print(root);
        std::cout << "\n\n";

        // --- Access values ---
        std::cout << "Name: " << root["name"].asString() << "\n";
        std::cout << "College: " << root["profile"]["college"].asString() << "\n";
        std::cout << "First mark: " << root["marks"][0].asNumber() << "\n";

        // --- Modify values ---
        root["age"] = JSONValue(21.0);
        root["profile"]["college"] = JSONValue("Delhi University");
        root["marks"][1] = JSONValue(99.0);

        std::cout << "\n After modifications:\n";
        JSONPrinter::print(root);
        std::cout << "\n";

    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
    }

    return 0;
}






