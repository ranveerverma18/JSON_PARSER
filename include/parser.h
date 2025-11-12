#pragma once

#include<bits/stdc++.h>
#include "tokenizer.h"
using namespace std;
struct JSONValue; // forward declaring the structure

using JSONArray = std::vector<std::shared_ptr<JSONValue>>;
using JSONObject = std::unordered_map<std::string, std::shared_ptr<JSONValue>>;

struct JSONValue : std::variant<
    string,
    double,
    bool,
    nullptr_t,
    JSONArray,
    JSONObject
> {
    using variant::variant;

    // --- Accessors ---
    bool isString() const { return holds_alternative<string>(*this); }
    bool isNumber() const { return holds_alternative<double>(*this); }
    bool isBool()   const { return holds_alternative<bool>(*this); }
    bool isNull()   const { return holds_alternative<nullptr_t>(*this); }
    bool isArray()  const { return holds_alternative<JSONArray>(*this); }
    bool isObject() const { return holds_alternative<JSONObject>(*this); }

    const string& asString() const { return get<string>(*this); }
    double asNumber() const { return get<double>(*this); }
    bool asBool() const { return get<bool>(*this); }
    const JSONArray& asArray() const { return get<JSONArray>(*this); }
    const JSONObject& asObject() const { return get<JSONObject>(*this); }

    // --- Mutable accessors ---
    JSONArray& asArray() { return get<JSONArray>(*this); }
    JSONObject& asObject() { return get<JSONObject>(*this); }

    // --- Operator Overloads ---
    JSONValue& operator[](const string& key) {
        if (!isObject()) throw runtime_error("Not a JSON object");
        auto& obj = get<JSONObject>(*this);
        if (!obj.count(key))
            obj[key] = make_shared<JSONValue>(nullptr);
        return *obj[key];
    }

    JSONValue& operator[](size_t index) {
        if (!isArray()) throw runtime_error("Not a JSON array");
        auto& arr = get<JSONArray>(*this);
        if (index >= arr.size())
            throw runtime_error("Array index out of bounds");
        return *arr[index];
    }
};


class Parser {
private:
    vector<Token> tokens;
    size_t pos = 0;

public:
    Parser(const vector<Token>& t) : tokens(t) {}
    JSONValue parse();

private:
    bool isAtEnd() const;
    const Token& peek() const;
    const Token& advance();
    void expect(TokenType type, const string& msg);
    JSONValue parseValue();
    JSONValue parseObject();
    JSONValue parseArray();
};







