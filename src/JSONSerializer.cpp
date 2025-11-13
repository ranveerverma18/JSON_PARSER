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

