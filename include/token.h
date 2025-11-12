#pragma once
#include <string>

enum class TokenType {
    LBRACE, RBRACE,
    LBRACKET, RBRACKET,
    COLON, COMMA,
    STRING, NUMBER,
    TRUE, FALSE, NUL,
    END_OF_FILE
};

// Representing one token: e.g. STRING("Ranveer"), NUMBER(20)
struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string& v = "")
        : type(t), value(v) {}
};
