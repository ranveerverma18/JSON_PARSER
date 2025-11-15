#pragma once
#include <string>
#include "parser.h"   

class JSONSerializer {
public:
// serialize():
// Converts the JSON tree (JSONValue) back into JSON text.
//
// STRING values MUST be escaped (using escapeString()).
// Otherwise characters like newline, tab, quotes or backslash
// would break the JSON format on output.
//
    static std::string serialize(const JSONValue& value, int indent = 0);
    static std::string serialize(const std::shared_ptr<JSONValue>& ptr, int indent = 0);
    static std::string serializeCompact(const JSONValue& value);

private:
    static std::string indentString(int indent);
    static std::string escapeString(const std::string& s);
};

// ------------------------------------------------------------
// ESCAPING STRING FOR JSON OUTPUT
//
// After parsing, JSONValue stores the REAL in-memory characters:
//     '\n'  '\t'  '\b'  '"'  '\\'
//
// But when writing JSON text back to a file or console, these
// characters MUST be converted back into JSON escape sequences.
//
// Example:
//     in memory:   Hello<newline>World
//     JSON text:   "Hello\nWorld"
//
// Without this, the serializer would output INVALID JSON.
//
// This function(escapeString) guarantees the JSON we output is syntactically valid.
// ------------------------------------------------------------
