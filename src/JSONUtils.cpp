#include "../include/JSONUtils.h"

std::string JSONUtils::escapeString(const std::string& s) {
    std::ostringstream out;

    for (char c : s) {
        switch (c) {
            case '\"': out << "\\\""; break;
            case '\\': out << "\\\\"; break;
            case '\b': out << "\\b";  break;
            case '\f': out << "\\f";  break;
            case '\n': out << "\\n";  break;
            case '\r': out << "\\r";  break;
            case '\t': out << "\\t";  break;

            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    out << "\\u"
                        << std::uppercase << std::hex
                        << std::setw(4) << std::setfill('0')
                        << (int)c;
                } else {
                    out << c;
                }
        }
    }
    return out.str();
}

std::string JSONUtils::formatNumber(double value) {
    std::ostringstream ss;
    
    // Check if it's an integer value
    if (value == static_cast<long long>(value)) {
        ss << static_cast<long long>(value);
    } else {
        // Use high precision for floating point
        ss << std::setprecision(17) << value;
    }
    
    return ss.str();
}
