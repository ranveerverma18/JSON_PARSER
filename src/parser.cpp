#include "../include/parser.h"
#include <iostream>
#include <memory>
#include <cstdlib>
#include <sstream>

// Helper: friendly token type name (for messages)
static std::string tokenTypeName(TokenType t) {
    switch (t) {
        case TokenType::LBRACE:   return "'{'";
        case TokenType::RBRACE:   return "'}'";
        case TokenType::LBRACKET: return "'['";
        case TokenType::RBRACKET: return "']'";
        case TokenType::COLON:    return "':'";
        case TokenType::COMMA:    return "','";
        case TokenType::STRING:   return "string";
        case TokenType::NUMBER:   return "number";
        case TokenType::TRUE:     return "'true'";
        case TokenType::FALSE:    return "'false'";
        case TokenType::NUL:      return "'null'";
        case TokenType::END_OF_FILE: return "<EOF>";
    }
    return "token";
}

bool Parser::isAtEnd() const {
    return pos >= tokens.size();
}

const Token& Parser::peek() const {
    if (pos >= tokens.size()) {
        // If no tokens available, provide best-effort position (end of input)
        if (!tokens.empty()) {
            const Token& last = tokens.back();
            throw JSONParseError("Unexpected end of input", last.line, last.column);
        } else {
            throw JSONParseError("Unexpected end of input", 1, 1);
        }
    }
    return tokens[pos];
}

const Token& Parser::advance() {
    if (pos >= tokens.size()) {
        if (!tokens.empty()) {
            const Token& last = tokens.back();
            throw JSONParseError("Unexpected end of input", last.line, last.column);
        } else {
            throw JSONParseError("Unexpected end of input", 1, 1);
        }
    }
    return tokens[pos++];
}

void Parser::expect(TokenType type, const std::string& expectedMessage) {
    if (isAtEnd() || peek().type != type) {
        if (isAtEnd()) {
            if (!tokens.empty()) {
                const Token& last = tokens.back();
                throw JSONParseError(expectedMessage + " but reached end of input", last.line, last.column);
            } else {
                throw JSONParseError(expectedMessage + " but reached end of input", 1, 1);
            }
        } else {
            const Token& t = peek();
            std::ostringstream ss;
            ss << expectedMessage << " but found " << tokenTypeName(t.type);
            throw JSONParseError(ss.str(), t.line, t.column);
        }
    }
    advance();
}

JSONValue Parser::parse() {
    JSONValue v = parseValue();
    // Accept if there are trailing tokens that aren't just EOF-like — if extra tokens present, it's an error.
    if (!isAtEnd()) {
        const Token& t = peek();
        std::ostringstream ss;
        ss << "Unexpected token after root value: " << tokenTypeName(t.type);
        throw JSONParseError(ss.str(), t.line, t.column);
    }
    return v;
}

JSONValue Parser::parseValue() {
    if (isAtEnd()) throw JSONParseError("Unexpected end while parsing value", 1, 1);

    const Token &t = peek();
    switch (t.type) {
        case TokenType::LBRACE:  return parseObject();
        case TokenType::LBRACKET: return parseArray();
        case TokenType::STRING: {
            advance();
            return JSONValue(t.value);
        }
        case TokenType::NUMBER: {
            advance();
            try {
                double val = std::stod(t.value);
                return JSONValue(val);
            } catch (...) {
                throw JSONParseError("Invalid number format: " + t.value, t.line, t.column);
            }
        }
        case TokenType::TRUE:
            advance();
            return JSONValue(true);
        case TokenType::FALSE:
            advance();
            return JSONValue(false);
        case TokenType::NUL:
            advance();
            return JSONValue(nullptr);
        default:
            throw JSONParseError("Unexpected token when parsing value: " + tokenTypeName(t.type), t.line, t.column);
    }
}

JSONValue Parser::parseObject() {
    // consume '{'
    expect(TokenType::LBRACE, "Expected '{' to start object");

    JSONObject object; 

    // empty object
    if (!isAtEnd() && peek().type == TokenType::RBRACE) {
        advance();
        return JSONValue(object);
    }

    while (true) {
        if (isAtEnd()) {
            throw JSONParseError("Unterminated object — expected string key or '}'", 1, 1);
        }

        if (peek().type != TokenType::STRING) {
            const Token &t = peek();
            throw JSONParseError("Expected string as object key", t.line, t.column);
        }

        // key
        const Token &keyTok = peek();
        std::string key = keyTok.value;
        advance(); // consume key

        // colon
        expect(TokenType::COLON, "Expected ':' after object key");

        // value
        JSONValue value = parseValue();
        object.emplace(std::move(key), std::make_shared<JSONValue>(std::move(value)));

        // comma or end
        if (!isAtEnd() && peek().type == TokenType::COMMA) {
            advance(); // consume comma and continue
            continue;
        } else if (!isAtEnd() && peek().type == TokenType::RBRACE) {
            advance(); // consume '}'
            break;
        } else {
            if (isAtEnd()) {
                throw JSONParseError("Unterminated object — expected ',' or '}'", 1, 1);
            } else {
                const Token &t = peek();
                throw JSONParseError("Expected ',' or '}' after object pair", t.line, t.column);
            }
        }
    }

    return JSONValue(object);
}

JSONValue Parser::parseArray() {
    // consume '['
    expect(TokenType::LBRACKET, "Expected '[' to start array");

    JSONArray arr; 

    // empty array
    if (!isAtEnd() && peek().type == TokenType::RBRACKET) {
        advance();
        return JSONValue(arr);
    }

    while (true) {
        JSONValue v = parseValue();
        arr.push_back(std::make_shared<JSONValue>(std::move(v)));

        // comma or end
        if (!isAtEnd() && peek().type == TokenType::COMMA) {
            advance();
            continue;
        } else if (!isAtEnd() && peek().type == TokenType::RBRACKET) {
            advance();
            break;
        } else {
            if (isAtEnd()) {
                throw JSONParseError("Unterminated array — expected ',' or ']'", 1, 1);
            } else {
                const Token &t = peek();
                throw JSONParseError("Expected ',' or ']' after array element", t.line, t.column);
            }
        }
    }

    return JSONValue(arr);
}



