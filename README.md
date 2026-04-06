# 🧩 JSON_PARSER

A compact C++17 JSON parser toolkit with:
- Tokenizer
- Recursive-descent parser
- JSON AST (JSONValue)
- Pretty and compact serializer
- JSON path navigation and mutation helpers
- CLI commands for real file workflows
- Optional Flask API wrapper for HTTP usage

This project is built for learning, extension, and practical tooling.

## 🎯 Why This Project

Most JSON parsers feel like black boxes. This one is intentionally readable.

Pipeline:
Input JSON text -> Tokenizer -> Parser -> JSONValue tree -> Serializer / Printer / Navigator

The codebase keeps responsibilities separate so each stage is easier to understand and debug.

## ✨ Key Features

- Full core JSON types: object, array, string, number, boolean, null
- Token line and column tracking for better error reporting
- Pretty formatting and minified serialization
- Path-based get and set operations
- File-based command line workflow
- Flask endpoints that call the same C++ binary
- Minimal dependencies (C++ standard library, Flask for API wrapper)

## 🗂️ Project Structure

JSON_PARSER/
- include/
  - FileUtils.h
  - JSONNavigator.h
  - JSONPath.h
  - JSONPrinter.h
  - JSONSerializer.h
  - parser.h
  - token.h
  - tokenizer.h
- src/
  - FileUtils.cpp
  - JSONNavigator.cpp
  - JSONPath.cpp
  - JSONprinter.cpp
  - JSONSerializer.cpp
  - parser.cpp
  - main.cpp
- server.py
- Dockerfile
- requirements.txt
- test.json
- test_pretty.json

Notes:
- Header uses JSONPrinter.h.
- Source file is currently named JSONprinter.cpp.

## 🛠️ Build And Run (Windows)

Prerequisites:
- Git
- g++ with C++17 support (MinGW/MSYS2), or MSVC

### 1) Clone The Repository

```bat
git clone <your-repo-url>
cd JSON_PARSER
```

### 2) Build The CLI (g++)

```bat
g++ -std=c++17 -Iinclude src/*.cpp -o json_parser.exe
```

### 3) Build The CLI (MSVC Developer Command Prompt)

```bat
cl /EHsc /std:c++17 /I include src\*.cpp /Fe:json_parser.exe
```

### 4) Run It

```bat
json_parser.exe validate test.json
json_parser.exe pretty test.json
json_parser.exe minify test.json
```

## 💻 CLI Usage

Syntax:
```bat
json_parser.exe <command> <input.json> [extra args]
```

Commands:

| Command | What it does |
|---|---|
| `pretty <input.json>` | Writes formatted output to `<input>_pretty.json` (or prints with `stdout`) |
| `minify <input.json>` | Writes compact output to `<input>_minified.json` (or prints with `stdout`) |
| `validate <input.json>` | Prints `[OK] Valid JSON` when successful |
| `show <input.json>` | Prints parsed JSON using `JSONPrinter` |
| `get <input.json> <path>` | Prints value at path |
| `set <input.json> <path> <value>` | Updates value in file (or prints with trailing `stdout`) |

Examples:
```bat
json_parser.exe validate test.json
json_parser.exe pretty test.json
json_parser.exe pretty test.json stdout
json_parser.exe minify test.json stdout
json_parser.exe get test.json profile.name
json_parser.exe set test.json profile.age 22
```

## 📚 API Summary (From Current Headers)

### 🧠 parser.h

- struct JSONValue : std::variant<string, double, bool, nullptr_t, JSONArray, JSONObject>
- class Parser
  - Parser(const std::vector<Token>&)
  - JSONValue parse()
- class JSONParseError : std::runtime_error

### 🔎 tokenizer.h and token.h

- class Tokenizer
  - Tokenizer(const std::string&)
  - std::vector<Token> tokenize()
- enum class TokenType
  - LBRACE, RBRACE, LBRACKET, RBRACKET, COLON, COMMA
  - STRING, NUMBER, TRUE, FALSE, NUL, END_OF_FILE
- struct Token
  - TokenType type
  - std::string value
  - int line
  - int column

### 🧾 JSONSerializer.h

- class JSONSerializer
  - static std::string serialize(const JSONValue&, int indent = 0)
  - static std::string serialize(const std::shared_ptr<JSONValue>&, int indent = 0)
  - static std::string serializeCompact(const JSONValue&)

### 🖨️ JSONPrinter.h

- class JSONPrinter
  - static void print(const JSONValue&, int indent = 0)

### 🧭 JSONPath.h and JSONNavigator.h

- struct PathElement
- class JSONPath
  - static std::vector<PathElement> parse(const std::string& path)
- class JSONNavigator
  - static JSONValue& get(JSONValue& root, const std::vector<PathElement>& path)

### 📁 FileUtils.h

- class FileUtils
  - static std::string readFile(const std::string& path)
  - static void writeFile(const std::string& path, const std::string& content)

## 🌐 Flask API Wrapper

File: server.py

The Flask app executes the compiled C++ binary and returns JSON responses.

Endpoints:
- POST /validate
  - Body: raw JSON text
- POST /pretty
  - Body: raw JSON text
- POST /minify
  - Body: raw JSON text
- POST /show
  - Body: raw JSON text
- POST /get
  - Body: { "json": "...", "path": "..." }
- POST /set
  - Body: { "json": "...", "path": "...", "value": "..." }

Response shape:
- { "result": "...", "error": "..." }

## 🐳 Docker Usage

Current Dockerfile does the following:
- Uses ubuntu:22.04
- Installs g++, python3, pip
- Compiles parser binary to /app/json_parser
- Installs Flask
- Exposes port 5000
- Runs server.py

Build:
- docker build -t json-parser-api .

Run:
- docker run --rm -p 5000:5000 json-parser-api

## 🚨 Error Handling Highlights

- Parser throws JSONParseError with line and column.
- Runtime errors are surfaced for invalid paths, bad tokens, and malformed input.
- CLI prints contextual parse errors for easier debugging.

## 🛣️ Suggested Roadmap

- Add formal tests (tokenizer, parser, serializer, path navigation)
- Add CMakeLists.txt for reproducible cross-platform builds
- Add CI workflow for build and test validation
- Extend number parsing and literal handling in set command
- Add benchmark script for large JSON files
