#include "JSONSerializer.h"
#include <sstream>

using namespace std;

string JSONSerializer::indentString(int indent) {
    return string(indent * 2, ' ');
}

string JSONSerializer::serialize(const JSONValue& value, int indent) {
    // STRING
    if (value.isString()) {
        return "\"" + value.asString() + "\"";
    }

    // NUMBER
    if (value.isNumber()) {
        ostringstream ss;
        ss << value.asNumber();
        return ss.str();
    }

    // BOOLEAN
    if (value.isBool()) {
        return value.asBool() ? "true" : "false";
    }

    // NULL
    if (value.isNull()) {
        return "null";
    }

    // ARRAY
    if (value.isArray()) {
        const JSONArray& arr = value.asArray();
        ostringstream out;
        out << "[\n";

        for (size_t i = 0; i < arr.size(); i++) {
            out << indentString(indent + 1)
                << serialize(arr[i], indent + 1);

            if (i + 1 < arr.size())
                out << ",";

            out << "\n";
        }

        out << indentString(indent) << "]";
        return out.str();
    }

    // OBJECT
    if (value.isObject()) {
        const JSONObject& obj = value.asObject();
        ostringstream out;
        out << "{\n";

        size_t count = 0;
        for (auto& p : obj) {
            out << indentString(indent + 1)
                << "\"" << p.first << "\": "
                << serialize(p.second, indent + 1);

            if (++count < obj.size())
                out << ",";

            out << "\n";
        }

        out << indentString(indent) << "}";
        return out.str();
    }
    return "";
}

string JSONSerializer::serialize(const shared_ptr<JSONValue>& ptr, int indent) {
    return serialize(*ptr, indent);
}


/* 
====================== JSON SERIALIZER — RECURSION TREE ======================

This is how the serializer calls itself recursively for a JSON like:

{
  "profile": {
    "college": "Delhi University",
    "year": 3
  },
  "marks": [98, 99, 93],
  "nickname": null,
  "height": -5.8,
  "student": true,
  "age": 21,
  "name": "Ranveer"
}

CALL TREE (indent value shown in parentheses):

serialize(root, 0)                  // OBJECT
│
├── serialize(profile_value, 1)
│   └── serialize(profile_object, 2)
│       ├── serialize("Delhi University", 3)
│       └── serialize(3, 3)
│
├── serialize(marks_value, 1)
│   └── serialize(marks_array, 2)
│       ├── serialize(98, 3)
│       ├── serialize(99, 3)
│       └── serialize(93, 3)
│
├── serialize(null, 1)
├── serialize(-5.8, 1)
├── serialize(true, 1)
├── serialize(21, 1)
└── serialize("Ranveer", 1)

NOTES:
- indent = depth level inside nested objects/arrays
- indentString(indent) prints (indent * 2) spaces
- Arrays and objects increase indent by +1 for their contents
- Each call returns a string to its parent, building the final JSON
==============================================================================
*/
