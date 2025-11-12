#include <bits/stdc++.h>
#include "../include/token.h"
#include "../include/tokenizer.h"
using namespace std;

int main() {
    string json = R"({
        "name": "Ranveer",
        "age": 20,
        "student": true,
        "nickname": null,
        "height": -5.8
    })";

    Tokenizer tokenizer(json);

    try {
        vector<Token> tokens = tokenizer.tokenize();

        for (const auto& token : tokens) {
            switch (token.type) {
                case TokenType::LBRACE: cout << "LBRACE\n"; break;
                case TokenType::RBRACE: cout << "RBRACE\n"; break;
                case TokenType::LBRACKET: cout << "LBRACKET\n"; break;
                case TokenType::RBRACKET: cout << "RBRACKET\n"; break;
                case TokenType::COLON: cout << "COLON\n"; break;
                case TokenType::COMMA: cout << "COMMA\n"; break;
                case TokenType::STRING: cout << "STRING(" << token.value << ")\n"; break;
                case TokenType::NUMBER: cout << "NUMBER(" << token.value << ")\n"; break;
                case TokenType::TRUE: cout << "TRUE\n"; break;
                case TokenType::FALSE: cout << "FALSE\n"; break;
                case TokenType::NUL: cout << "NULL\n"; break;
                default: break;
            }
        }

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}


