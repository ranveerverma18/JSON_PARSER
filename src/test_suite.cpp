#include <iostream>
#include <vector>
#include <string>
#include <cassert>

#include "../include/tokenizer.h"
#include "../include/parser.h"
#include "../include/JSONSerializer.h"

int passedTests = 0;
int failedTests = 0;

void test(const std::string& name, bool condition) {
    if (condition) {
        std::cout << "✓ " << name << "\n";
        passedTests++;
    } else {
        std::cout << "✗ " << name << "\n";
        failedTests++;
    }
}

void testParseSuccess(const std::string& name, const std::string& json) {
    try {
        Tokenizer t(json);
        auto tokens = t.tokenize();
        Parser p(tokens);
        p.parse();
        test(name, true);
    } catch (...) {
        test(name, false);
    }
}

void testParseFailure(const std::string& name, const std::string& json) {
    try {
        Tokenizer t(json);
        auto tokens = t.tokenize();
        Parser p(tokens);
        p.parse();
        test(name, false); // Should have thrown
    } catch (...) {
        test(name, true); // Expected to fail
    }
}

void testRoundTrip(const std::string& name, const std::string& json) {
    try {
        Tokenizer t(json);
        auto tokens = t.tokenize();
        Parser p(tokens);
        JSONValue root = p.parse();
        
        std::string serialized = JSONSerializer::serializeCompact(root);
        
        // Parse again
        Tokenizer t2(serialized);
        auto tokens2 = t2.tokenize();
        Parser p2(tokens2);
        p2.parse();
        
        test(name, true);
    } catch (...) {
        test(name, false);
    }
}

int main() {
    std::cout << "\n========================================\n";
    std::cout << "   JSON PARSER COMPLIANCE TEST SUITE\n";
    std::cout << "========================================\n\n";
    
    // Basic types
    std::cout << "--- Basic Types ---\n";
    testParseSuccess("Empty object", "{}");
    testParseSuccess("Empty array", "[]");
    testParseSuccess("String", "\"hello\"");
    testParseSuccess("Number integer", "42");
    testParseSuccess("Number negative", "-42");
    testParseSuccess("Number decimal", "3.14");
    testParseSuccess("Number negative decimal", "-3.14");
    testParseSuccess("Boolean true", "true");
    testParseSuccess("Boolean false", "false");
    testParseSuccess("Null", "null");
    
    // Strings with escapes
    std::cout << "\n--- String Escaping ---\n";
    testParseSuccess("Escaped quote", R"("He said \"Hi\"")");
    testParseSuccess("Escaped backslash", R"("C:\\path\\file")");
    testParseSuccess("Escaped newline", R"("Line1\nLine2")");
    testParseSuccess("Escaped tab", R"("Col1\tCol2")");
    testParseSuccess("Escaped backspace", R"("Test\b")");
    testParseSuccess("Escaped form feed", R"("Test\f")");
    testParseSuccess("Escaped carriage return", R"("Test\r")");
    testParseSuccess("Multiple escapes", R"("\"\\\/\b\f\n\r\t")");
    
    // Objects
    std::cout << "\n--- Objects ---\n";
    testParseSuccess("Simple object", R"({"key":"value"})");
    testParseSuccess("Multiple keys", R"({"a":1,"b":2,"c":3})");
    testParseSuccess("Nested object", R"({"outer":{"inner":"value"}})");
    testParseSuccess("Object with array", R"({"arr":[1,2,3]})");
    testParseSuccess("Object with whitespace", "{ \"key\" : \"value\" }");
    
    // Arrays
    std::cout << "\n--- Arrays ---\n";
    testParseSuccess("Simple array", "[1,2,3]");
    testParseSuccess("Mixed types", R"([1,"two",true,null])");
    testParseSuccess("Nested arrays", "[[1,2],[3,4]]");
    testParseSuccess("Array of objects", R"([{"a":1},{"b":2}])");
    testParseSuccess("Array with whitespace", "[ 1 , 2 , 3 ]");
    
    // Deep nesting
    std::cout << "\n--- Deep Nesting ---\n";
    testParseSuccess("Nested 10 levels", R"({"a":{"b":{"c":{"d":{"e":{"f":{"g":{"h":{"i":{"j":"deep"}}}}}}}}}}})");
    testParseSuccess("Array nested 10 levels", "[[[[[[[[[[\"deep\"]]]]]]]]]]");
    
    // Whitespace handling
    std::cout << "\n--- Whitespace ---\n";
    testParseSuccess("Leading whitespace", "   {}");
    testParseSuccess("Trailing whitespace", "{}   ");
    testParseSuccess("Newlines", "{\n\"key\":\n\"value\"\n}");
    testParseSuccess("Tabs", "{\t\"key\":\t\"value\"\t}");
    
    // Round-trip tests
    std::cout << "\n--- Round-Trip (Parse → Serialize → Parse) ---\n";
    testRoundTrip("Round-trip object", R"({"name":"John","age":30,"active":true})");
    testRoundTrip("Round-trip array", "[1,2,3,4,5]");
    testRoundTrip("Round-trip nested", R"({"users":[{"id":1,"name":"Alice"},{"id":2,"name":"Bob"}]})");
    testRoundTrip("Round-trip with escapes", R"({"text":"Line1\nLine2\tTabbed"})");
    
    // Error cases
    std::cout << "\n--- Error Handling ---\n";
    testParseFailure("Trailing comma in object", R"({"a":1,})");
    testParseFailure("Trailing comma in array", "[1,2,]");
    testParseFailure("Missing colon", R"({"key" "value"})");
    testParseFailure("Missing comma", R"({"a":1 "b":2})");
    testParseFailure("Unquoted key", "{key:value}");
    testParseFailure("Single quotes", "{'key':'value'}");
    testParseFailure("Unterminated string", R"({"key":"value)");
    testParseFailure("Unterminated object", R"({"key":"value")");
    testParseFailure("Unterminated array", "[1,2,3");
    testParseFailure("Invalid number", "123.45.67");
    testParseFailure("Invalid keyword", "tru");
    testParseFailure("Extra closing brace", R"({"key":"value"}})");
    testParseFailure("Extra closing bracket", "[1,2,3]]");
    
    // Edge cases
    std::cout << "\n--- Edge Cases ---\n";
    testParseSuccess("Empty string", R"("")");
    testParseSuccess("Zero", "0");
    testParseSuccess("Negative zero", "-0");
    testParseSuccess("Large number", "999999999999");
    testParseSuccess("Small decimal", "0.0001");
    testParseSuccess("Unicode in string", R"("Hello 世界")");
    
    // Summary
    std::cout << "\n========================================\n";
    std::cout << "RESULTS:\n";
    std::cout << "  Passed: " << passedTests << "\n";
    std::cout << "  Failed: " << failedTests << "\n";
    std::cout << "  Total:  " << (passedTests + failedTests) << "\n";
    
    if (failedTests == 0) {
        std::cout << "\n✓ ALL TESTS PASSED!\n";
    } else {
        std::cout << "\n✗ SOME TESTS FAILED\n";
    }
    std::cout << "========================================\n\n";
    
    return failedTests > 0 ? 1 : 0;
}
