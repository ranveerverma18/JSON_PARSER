#pragma once   //the function of this is that it prevents multiple inclusions of the same header file in a single compilation unit.

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <memory>
#include <stdexcept>

#include "token.h"   // only need token definitions here

using namespace std;

struct JSONValue; // forward declaring the structure

using JSONArray = vector<shared_ptr<JSONValue>>;
using JSONObject = unordered_map<string, shared_ptr<JSONValue>>;

struct JSONValue : variant<
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

    // --- Mutable Accessors ---
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

// ----------------------
// JSONParseError
// ----------------------
// Thrown by the parser when a syntactic error is encountered.
// Contains a user-friendly message and the line/column where the error occurred.
class JSONParseError : public std::runtime_error {
public:
    int line;
    int column;

    JSONParseError(const std::string &msg, int ln = 1, int col = 1)
        : std::runtime_error(msg), line(ln), column(col) {}
};

// ----------------------
// Parser
// ----------------------
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
    void expect(TokenType type, const std::string& expectedMessage);
    JSONValue parseValue();
    JSONValue parseObject();
    JSONValue parseArray();
};





/* 
====================== JSON PARSER — RECURSIVE DESCENT ======================

This Parser consumes the list of tokens produced by Tokenizer and builds
a recursive JSONValue tree using the following grammar:

GRAMMAR (JSON Standard):

  value
      → object
      | array
      | string
      | number
      | true | false | null

  object
      → '{' '}'
      | '{' members '}'

  members
      → pair
      | pair ',' members

  pair
      → STRING ':' value

  array
      → '[' ']'
      | '[' elements ']'

  elements
      → value
      | value ',' elements

PARSER FUNCTIONS & WHAT THEY PARSE:

  parse()          → starts parsing at 'value'
  parseValue()     → detects type (string/num/obj/arr/bool/null)
  parseObject()    → parses {...} and key-value pairs recursively
  parseArray()     → parses [...] and values recursively

CONTROL FLOW EXAMPLE:
  - parse() calls parseValue()
  - parseValue() sees '{' → calls parseObject()
  - parseObject() sees key → calls parseValue() for its value
  - nested objects/arrays trigger more recursive calls

The output of parse() is a fully built JSONValue tree that
matches the structure of the input JSON text.

=============================================================================
*/





/* 
====================== JSONVALUE — INTERNAL JSON TREE NODE ======================

JSONValue represents ANY JSON element using std::variant:

  JSONValue = variant<
      string,         // JSON string
      double,         // JSON number
      bool,           // true / false
      nullptr_t,      // null
      JSONArray,      // vector<shared_ptr<JSONValue>>
      JSONObject      // unordered_map<string, shared_ptr<JSONValue>>
  >

ARRAY = JSONArray = vector<shared_ptr<JSONValue>>
OBJECT = JSONObject = unordered_map<string, shared_ptr<JSONValue>>

REASONS FOR USING shared_ptr<JSONValue>:
  - Avoid copying large JSON subtrees
  - Allows recursive structures naturally
  - Makes operator[] easy: returns references into object/array

ACCESSORS:
  - isString(), isNumber(), isArray() ...
  - asString(), asArray() ...

OPERATOR[]:
  - obj["key"] gives reference to its JSONValue
  - arr[index] gives reference to element

This structure makes JSON recursion, modification, and serialization simple.

===============================================================================
*/



