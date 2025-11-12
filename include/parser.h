#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <memory>
#include "tokenizer.h"

struct JSONValue; // forward declare

using JSONArray = std::vector<std::shared_ptr<JSONValue>>;
using JSONObject = std::unordered_map<std::string, std::shared_ptr<JSONValue>>;

struct JSONValue : std::variant<
    std::string,
    double,
    bool,
    std::nullptr_t,
    JSONArray,
    JSONObject
> {
    using variant::variant;
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t pos = 0;

public:
    Parser(const std::vector<Token>& t) : tokens(t) {}
    JSONValue parse();

private:
    bool isAtEnd() const;
    const Token& peek() const;
    const Token& advance();
    void expect(TokenType type, const std::string& msg);
    JSONValue parseValue();
    JSONValue parseObject();
    JSONValue parseArray();
};







