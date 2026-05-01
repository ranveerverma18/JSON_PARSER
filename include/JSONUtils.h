#pragma once
#include <string>
#include <sstream>
#include <iomanip>

class JSONUtils {
public:
    // Escape string for valid JSON output
    static std::string escapeString(const std::string& s);
    
    // Format number with proper precision
    static std::string formatNumber(double value);
};
