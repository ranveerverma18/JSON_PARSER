#include "../include/parser.h"
#include <iostream>
#include <memory>
#include <cstdlib>

bool Parser::isAtEnd() const {
    return pos >= tokens.size() || tokens[pos].type == TokenType::END_OF_FILE;
}

const Token& Parser::peek() const {
    if (pos >= tokens.size()) throw std::runtime_error("Unexpected end of tokens (peek)");
    return tokens[pos];
}

const Token& Parser::advance() {
    if (pos >= tokens.size()) throw std::runtime_error("Unexpected end of tokens (advance)");
    return tokens[pos++];
}

void Parser::expect(TokenType type, const std::string& msg) {
    if (isAtEnd() || peek().type != type)
        throw std::runtime_error(msg);
    advance();
}

JSONValue Parser::parse() {
    return parseValue();
}

JSONValue Parser::parseValue() {
    if (isAtEnd()) throw std::runtime_error("Unexpected end while parsing value");

    const Token &t = peek();
    switch (t.type) {
        case TokenType::LBRACE:  return parseObject();
        case TokenType::LBRACKET: return parseArray();
        case TokenType::STRING:
            advance();
            return JSONValue(t.value);
        case TokenType::NUMBER: {
            advance();
            double val = std::stod(t.value);
            return JSONValue(val);
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
            throw std::runtime_error("Unexpected token when parsing value");
    }
}

JSONValue Parser::parseObject() {
    expect(TokenType::LBRACE, "Expected '{' to start object");

    JSONObject object; // ✅ uses shared_ptr<JSONValue>

    if (!isAtEnd() && peek().type == TokenType::RBRACE) {
        advance();
        return JSONValue(object);
    }

    while (true) {
        if (isAtEnd() || peek().type != TokenType::STRING)
            throw std::runtime_error("Expected string as object key");

        std::string key = peek().value;
        advance(); // consume key

        expect(TokenType::COLON, "Expected ':' after object key");

        JSONValue value = parseValue();
        object.emplace(std::move(key), std::make_shared<JSONValue>(std::move(value)));

        if (!isAtEnd() && peek().type == TokenType::COMMA) {
            advance();
            continue;
        } else if (!isAtEnd() && peek().type == TokenType::RBRACE) {
            advance();
            break;
        } else {
            throw std::runtime_error("Expected ',' or '}' after object pair");
        }
    }

    return JSONValue(object);
}

JSONValue Parser::parseArray() {
    expect(TokenType::LBRACKET, "Expected '[' to start array");

    JSONArray arr; // ✅ uses shared_ptr<JSONValue>

    if (!isAtEnd() && peek().type == TokenType::RBRACKET) {
        advance();
        return JSONValue(arr);
    }

    while (true) {
        JSONValue v = parseValue();
        arr.push_back(std::make_shared<JSONValue>(std::move(v)));

        if (!isAtEnd() && peek().type == TokenType::COMMA) {
            advance();
            continue;
        } else if (!isAtEnd() && peek().type == TokenType::RBRACKET) {
            advance();
            break;
        } else {
            throw std::runtime_error("Expected ',' or ']' after array element");
        }
    }

    return JSONValue(arr);
}


