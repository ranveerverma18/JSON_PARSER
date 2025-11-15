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
    int line=1;
    int column=1;
    Token(TokenType t, const std::string& v = "",int ln=1,int col=1)
        : type(t), value(v), line(ln), column(col) {}
};
