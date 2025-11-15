#pragma once
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>
#include "token.h"

// Tokenizer with line + column tracking
class Tokenizer {
private:
    std::string input;
    size_t pos = 0;      // index into input
    int line = 1;        // current line number
    int column = 1;      // current column number

public:
    Tokenizer(const std::string& text) : input(text) {}

    bool isAtEnd() const {
        return pos >= input.size();
    }

    char peek() const {
        return isAtEnd() ? '\0' : input[pos];
    }

    // Advance one character and update line + column
    char advance() {
        if (isAtEnd())
            return '\0';

        char c = input[pos++];

        if (c == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }

        return c;
    }

    // Looking at next next character without consuming
    char peekNext() const {
        return (pos + 1 >= input.size()) ? '\0' : input[pos + 1];
    }

    void skipWhitespace() {
        while (!isAtEnd() && std::isspace(peek())) {
            advance(); // advance handles line/column counting
        }
    }

    // ====================================================
    // Main token dispatcher
    // ====================================================
    Token nextToken() {
        skipWhitespace();

        int tokenLine = line;
        int tokenCol  = column;

        if (isAtEnd())
            return Token(TokenType::END_OF_FILE, "", tokenLine, tokenCol);

        char c = advance();

        switch (c) {
            case '{': return Token(TokenType::LBRACE,   "{", tokenLine, tokenCol);
            case '}': return Token(TokenType::RBRACE,   "}", tokenLine, tokenCol);
            case '[': return Token(TokenType::LBRACKET, "[", tokenLine, tokenCol);
            case ']': return Token(TokenType::RBRACKET, "]", tokenLine, tokenCol);
            case ':': return Token(TokenType::COLON,    ":", tokenLine, tokenCol);
            case ',': return Token(TokenType::COMMA,    ",", tokenLine, tokenCol);

            case '"':
                return stringToken(tokenLine, tokenCol);
        }

        if (isdigit(c) || c == '-')
            return numberToken(c, tokenLine, tokenCol);

        if (isalpha(c))
            return keywordToken(c, tokenLine, tokenCol);

        throw std::runtime_error(
            "Unexpected character '" + std::string(1, c) +
            "' at line " + std::to_string(tokenLine) +
            ", column " + std::to_string(tokenCol)
        );
    }

        // Handling the string tokens
    // ------------------------------------------------------------
// STRING TOKEN PARSING (CRITICAL)
//
// JSON strings may contain escaped characters: \" \\ \n \t \b etc.
// The OLD tokenizer treated every " as the end of the string,
// even when it was escaped like \".
//
// That caused the tokenizer to prematurely close the string when
// encountering \" inside text such as:
//
//      "He said \"Yo!\""
//
// and then the parser saw `Yo` as a separate keyword => ERROR.
//
// This updated stringToken() properly handles escape sequences.
// Only an UNESCAPED " ends the string.
//
// VERY IMPORTANT FOR VALID JSON PARSING.
// ------------------------------------------------------------
    Token stringToken(int tokenLine, int tokenCol) {
        std::string value;

        while (!isAtEnd()) {
            char c = advance();

            if (c == '\\') {
                if (isAtEnd())
                    throw std::runtime_error("Invalid escape at end of string at line "
                        + std::to_string(line) + ", column " + std::to_string(column));

                char next = advance();
                switch (next) {
                    case '"':  value += '\"'; break;
                    case '\\': value += '\\'; break;
                    case '/':  value += '/';  break;
                    case 'b':  value += '\b'; break;
                    case 'f':  value += '\f'; break;
                    case 'n':  value += '\n'; break;
                    case 'r':  value += '\r'; break;
                    case 't':  value += '\t'; break;

                    default:
                        throw std::runtime_error("Invalid escape sequence: \\" 
                            + std::string(1, next) +
                            " at line " + std::to_string(line) +
                            ", column " + std::to_string(column));
                }
            }
            else if (c == '"') {
                return Token(TokenType::STRING, value, tokenLine, tokenCol);
            }
            else {
                value += c;
            }
        }

        throw std::runtime_error("Unterminated string literal at line "
            + std::to_string(tokenLine) +
            ", column " + std::to_string(tokenCol));
    }

    // ====================================================
    // Parse NUMBER tokens
    // ====================================================
    Token numberToken(char firstChar, int tokenLine, int tokenCol) {
        std::string value(1, firstChar);
        bool hasDecimal = false;

        while (!isAtEnd()) {
            char c = peek();

            if (isdigit(c)) {
                value += advance();
            }
            else if (c == '.' && !hasDecimal) {
                hasDecimal = true;
                value += advance();
            }
            else break;
        }

        if (value.back() == '.')
            throw std::runtime_error("Invalid number format: " + value +
                " at line " + std::to_string(tokenLine) +
                ", column " + std::to_string(tokenCol));

        return Token(TokenType::NUMBER, value, tokenLine, tokenCol);
    }

    // ====================================================
    // Parse keywords: true / false / null
    // ====================================================
    Token keywordToken(char firstChar, int tokenLine, int tokenCol) {
        std::string word(1, firstChar);

        while (!isAtEnd() && isalpha(peek()))
            word += advance();

        if (word == "true")
            return Token(TokenType::TRUE, word, tokenLine, tokenCol);
        if (word == "false")
            return Token(TokenType::FALSE, word, tokenLine, tokenCol);
        if (word == "null")
            return Token(TokenType::NUL, word, tokenLine, tokenCol);

        throw std::runtime_error("Unexpected keyword \"" + word +
            "\" at line " + std::to_string(tokenLine) +
            ", column " + std::to_string(tokenCol));
    }

    // ====================================================
    // TOKENIZE ENTIRE INPUT
    // ====================================================
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




