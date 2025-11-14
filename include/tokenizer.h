#pragma once
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>
#include "token.h"
#include<bits/stdc++.h>

class Tokenizer {
private:
    std::string input;
    size_t pos = 0; // current position in string

public:
    Tokenizer(const std::string& text) : input(text) {}

    bool isAtEnd() const {
        return pos >= input.size();
    }

    char peek() const {
        return isAtEnd() ? '\0' : input[pos];
    }

    char advance() {
        return isAtEnd() ? '\0' : input[pos++];
    }

    void skipWhitespace() {
        while (!isAtEnd() && std::isspace(peek()))
            pos++;
    }

    // Recognize simple symbol tokens
    Token nextToken() {
    skipWhitespace();
    if (isAtEnd())
        return Token(TokenType::END_OF_FILE);

    char c = advance();

    switch (c) {
        case '{': return Token(TokenType::LBRACE);
        case '}': return Token(TokenType::RBRACE);
        case '[': return Token(TokenType::LBRACKET);
        case ']': return Token(TokenType::RBRACKET);
        case ':': return Token(TokenType::COLON);
        case ',': return Token(TokenType::COMMA);
        case '"': return stringToken(); // handle strings
    }

    // Numbers can start with digit or '-'
    if (isdigit(c) || c == '-') return numberToken(c);

    // Keywords like true, false, null start with letter
    if (isalpha(c)) return keywordToken(c);

    throw std::runtime_error(std::string("Unexpected character: ") + c);
}

    // Handling the string tokens
    Token stringToken() {
        std::string value;

    while (!isAtEnd() && peek() != '"') {
        value += advance();
    }

    if (isAtEnd()) throw std::runtime_error("Unterminated string literal");

    advance(); // Skip closing quote
    return Token(TokenType::STRING, value);
    }

    
    // Handling number tokens
    Token numberToken(char firstChar) {
        std::string value(1, firstChar);
        bool hasDecimal = false; 
        while(!isAtEnd()){
            char c = peek();
            if (isdigit(c)) {
                value += advance();
            } else if (c == '.' && !hasDecimal) {
                hasDecimal = true;
                value += advance();
            } else {
                break;
        }
    }
        if(value.back()=='.')
            throw std::runtime_error("Invalid number format: " + value);
        return Token(TokenType::NUMBER, value);
  }


    // Handling keywords: true, false, null
    Token keywordToken(char firstChar) {
        std::string word(1, firstChar);

    while (!isAtEnd() && isalpha(peek()))
        word += advance();

    if (word == "true") return Token(TokenType::TRUE);
    if (word == "false") return Token(TokenType::FALSE);
    if (word == "null") return Token(TokenType::NUL);

    throw std::runtime_error("Unexpected keyword: " + word);
    }


    // Collect all tokens
    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        Token token = nextToken();
        while (token.type != TokenType::END_OF_FILE) {
            tokens.push_back(token);
            token = nextToken();
        }
        return tokens;
    }
};


/* 
====================== TOKENIZER — HOW IT WORKS ======================

The Tokenizer reads the input string character-by-character and emits
a sequence of tokens that the Parser will consume.

MAIN TOKEN TYPES:
  - { , } , [ , ]
  - : , ,
  - STRING
  - NUMBER
  - true / false
  - null

HOW TOKENIZATION WORKS (STEP-BY-STEP):

1. Skip whitespace (space, tab, newline)

2. If a '{' is seen → emit TokenType::LBRACE
3. If a '[' is seen → emit TokenType::LBRACKET
4. If a ':' or ',' is seen → emit corresponding token

5. If a '"' is seen:
      begin reading a STRING
      read characters until next '"'
      (later improvements: handle escape sequences)

6. If a digit or '-' starts:
      read characters of NUMBER
      supports integers and decimals

7. If 't', 'f', or 'n' appears:
      read literal true / false / null

8. If unknown character appears → throw error

The output is a flat vector<Token> which the Parser reads sequentially.

======================================================================
*/
