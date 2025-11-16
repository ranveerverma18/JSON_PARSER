# 🗿JSON_PARSER

A compact, readable C++ JSON tokenizer, parser, navigator and serializer library with utilities and a small demo CLI. Designed for learning, embedding into other projects, and incremental extension. Targets modern C++ (C++17 recommended).

---

## 📚Table of Contents
- Overview
- Features
- Tech stack used
- Architecture
- Getting started (Quick build)
- Usage examples
- API summary (headers)
- Development & testing
- Troubleshooting
- Performance & limitations
- Roadmap
- Contributing
- License

---

## 🌟Overview
JSON_PARSER converts JSON text into an in-memory AST (`JSONValue`) and back. The project separates responsibilities into tokenizer, parser and serializer and provides helpers for file I/O and path-based navigation (`JSONPath` / `JSONNavigator`). The codebase is intentionally compact and dependency-free (standard library only) so it is easy to read, modify and embed.

Primary goals:
- Demonstrate a clear tokenizer → parser → AST → serializer pipeline in C++.
- Provide utilities for querying and printing JSON.
- Keep implementation portable and easy to build on Windows / Linux / macOS.

C++ Standard: C++17 (required).

---

## ✨Features
- Full JSON support: objects, arrays, strings, numbers, booleans, null.
- Tokenizer and parser separated for clarity and testability.
- Pretty (indented) and compact serialization with correct string escaping.
- JSONPath & JSONNavigator utilities for selecting/traversing AST nodes.
- File utilities (read/write) and a CLI demo (`main.cpp`).
- No external runtime dependencies — standard library only.
- Developer-friendly layout: header-only public API (include/) and implementations (src/).

---

## 🛠️ Tech stack used
- Language: Modern C++ (C++17)
- Compiler compatibility: g++ (MinGW/MSYS2), clang, MSVC (with /std:c++17)
- Build tools: g++/cl or CMake (recommended)
- Libraries: C++ Standard Library only (strings, containers, streams, smart pointers, optional/variant if used)

---

## 🏗️Architecture
Project layout (actual)
- include/
  - FileUtils.h        — file I/O helpers (read/write)
  - JSONNavigator.h    — traversal & querying helpers
  - JSONPath.h         — path-expression utilities
  - JSONprinter.h      — console pretty-printer API
  - JSONSerializer.h   — serialize / serializeCompact declarations
  - parser.h           — AST types and parser API
  - token.h            — token definitions and position info
  - tokenizer.h        — lexical analyzer interface
- src/
  - FileUtils.cpp
  - JSONNavigator.cpp
  - JSONPath.cpp
  - JSONprinter.cpp
  - JSONSerializer.cpp
  - parser.cpp
  - main.cpp           — demo CLI / example usage

Design principles
- Single responsibility: tokenizer ≠ parser ≠ serializer.
- Clear public API in headers; implementations in src/.
- Focus on readability, testability and safe defaults.

---

## 🚀Getting started (Quick build)

Prerequisites
- C++ compiler with C++17 support
- Optional: CMake (>= 3.10)
- Windows examples shown — Linux/macOS commands are analogous.

1) Clone repository (example local path)
```bat
cd C:\Users\Ranveer Verma\Desktop
git clone <your-repo-url> JSON_PARSER
cd JSON_PARSER
```

2) Quick compile with g++ (MinGW / MSYS2)
```bat
mkdir bin
g++ -std=c++17 -Iinclude -O2 src\*.cpp -o bin\json_parser.exe
# run demo
bin\json_parser.exe
```

3) Quick compile with MSVC (Developer Command Prompt)
```bat
mkdir bin
cl /EHsc /std:c++17 /I include src\*.cpp /Fe:bin\json_parser.exe
bin\json_parser.exe
```

4) Build with CMake (recommended)
Create `CMakeLists.txt` at project root (example below), then:
```bat
mkdir build
cd build
cmake ..
cmake --build . --config Release
# executable will be in build\ or specified output dir
```

Minimal CMakeLists.txt (paste into project root)
```cmake
cmake_minimum_required(VERSION 3.10)
project(JSON_PARSER LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(json_parser ${SRC})
```

Notes
- Ensure builds use C++17. Many source files use structured bindings and other modern features.
- Place compiled binary under `bin\` for convenience.

---

## ✒️Usage examples

Note: replace function names if your parser API differs slightly.

1) Parse a JSON string and pretty-print
```cpp
#include "parser.h"
#include "JSONSerializer.h"
#include <iostream>

int main() {
    std::string src = R"({"name":"Ranveer","age":21,"marks":[98,99,93]})";
    JSONValue root = parseJSON(src);                 // adapt to your API
    std::cout << JSONSerializer::serialize(root, 0) << std::endl;
    return 0;
}
```

2) Read from file, query a path and print compact
```cpp
#include "FileUtils.h"
#include "parser.h"
#include "JSONSerializer.h"
#include "JSONNavigator.h"
#include <iostream>

int main() {
    std::string text = FileUtils::readFile("data/sample.json");
    JSONValue root = parseJSON(text);
    auto node = JSONNavigator::select(root, "/profile/college");
    if (node) {
        std::cout << JSONSerializer::serializeCompact(*node) << std::endl;
    }
    return 0;
}
```

3) CLI usage (if implemented in `main.cpp`)
- Example: `bin\json_parser.exe examples/sample.json`
- Expected behavior: parse file, print pretty output, show sample navigator result.

---

## 📡API summary (by header)
Consult `include/*.h` for exact signatures. typical interface:

- parser.h
  - JSONValue — variant-like AST node representing object/array/string/number/bool/null
  - parseJSON(const std::string& text) -> JSONValue / parse result
  - parseFile(const std::string& path) -> JSONValue / optional

- tokenizer.h / token.h
  - enum TokenType { LeftBrace, RightBrace, LeftBracket, RightBracket, String, Number, True, False, Null, Colon, Comma, End, Error }
  - struct Token { TokenType type; std::string lexeme; size_t line, col; }

- JSONSerializer.h
  - static std::string JSONSerializer::serialize(const JSONValue& value, int indent = 0);
  - static std::string JSONSerializer::serializeCompact(const JSONValue& value);

- JSONprinter.h
  - static void JSONPrinter::print(const JSONValue& value, int indent = 0);

- JSONPath.h / JSONNavigator.h
  - JSONNavigator::select(const JSONValue& root, const std::string& path) -> optional<JSONValue>
  - Helpers: exists(), getAsString(), getAsNumber(), etc.

- FileUtils.h
  - static std::string FileUtils::readFile(const std::string& path);
  - static bool FileUtils::writeFile(const std::string& path, const std::string& content);

If you want, a one-page reference that extracts exact signatures from your headers can be generated.

---

## 🧪Development & testing

Recommended test matrix
- Tokenizer:
  - Escaped strings: \" \\ \b \f \n \r \t and unicode `\uXXXX`
  - Numbers: integers, floats, exponents, negative numbers
  - Invalid tokens (unterminated strings, bad escapes)
- Parser:
  - Round-trip: parse -> serialize -> parse equality
  - Syntax errors: clear line/column diagnostics
- Serializer:
  - Proper escaping behavior
  - Pretty indentation and compact output correctness
- Navigator:
  - Path resolution, array indexing, missing nodes handling

Test harness
- Use Catch2 or GoogleTest.
- Add a `tests/` folder and a CMake target `add_subdirectory(tests)`.
- Add CI (GitHub Actions) to run builds and tests on push/PR.

Example test command (after adding tests and CMake support):
```bat
cmake --build . --target run_tests
```

---

## 👁️‍🗨️Troubleshooting

Common issues and fixes
- Compiler error "expected ';'" near `[key, val]` in range-for:
  - Cause: compiler not using C++17.
  - Fix: add `-std=c++17` (g++/clang) or `/std:c++17` (MSVC).
- Linker errors (undefined references):
  - Ensure all `.cpp` files from `src/` are compiled and linked (especially `parser.cpp`).
- Incorrect escaping in serializer:
  - Inspect `JSONSerializer::escapeString()` and tokenizer string handling.
- Headers polluting global namespace:
  - Remove `using namespace std;` from headers; keep it in `.cpp` files only.

---

## ⚡Performance & limitations
- Parser and serializer are recursive; extremely deep JSON may cause stack overflow.
- Single-threaded tokenizer/parser; for very large files consider streaming or SAX-like processing.
- No schema validation is included — this project focuses on parsing, navigation and serialization.







