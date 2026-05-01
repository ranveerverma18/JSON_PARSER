# 🧩 JSON_PARSER

A high-performance C++17 JSON parser with full ECMA-404 compliance, featuring tokenization, recursive-descent parsing, AST manipulation, and HTTP API wrapper.

## 🎯 Performance Highlights

- **Peak Throughput**: 70+ MB/s on string-heavy workloads
- **Deep Nesting**: Handles 500+ nesting levels without stack overflow
- **Memory Efficiency**: 1.3x overhead on optimal structures
- **Test Coverage**: 57 ECMA-404 compliance test cases
- **Data Processed**: Validated across 3.5+ MB of diverse JSON structures

## ✨ Key Features

- Full JSON types: object, array, string, number, boolean, null
- Precise error reporting with line/column tracking
- Pretty formatting and minified serialization
- O(1) hash-map backed key access
- Path-based navigation and in-place mutation
- File-based CLI workflow
- Optional Flask API wrapper for HTTP usage
- Comprehensive benchmark suite

## 📁 Project Structure

```
JSON_PARSER/
├── include/              # Header files
│   ├── parser.h         # Core parser and JSONValue AST
│   ├── tokenizer.h      # Lexical analyzer
│   ├── JSONSerializer.h # JSON output
│   ├── JSONPrinter.h    # Debug printing
│   ├── JSONNavigator.h  # Path-based access
│   ├── JSONPath.h       # Path parsing
│   ├── JSONUtils.h      # Shared utilities
│   ├── FileUtils.h      # File I/O
│   └── token.h          # Token definitions
│
├── src/                 # Implementation files
│   ├── main.cpp         # CLI entry point
│   ├── parser.cpp       # Parser implementation
│   ├── benchmark.cpp    # Performance testing
│   ├── test_suite.cpp   # Compliance tests
│   └── ...              # Other implementations
│
├── tests/               # Test JSON files
│   ├── test.json
│   └── test_pretty.json
│
├── benchmarks/          # Benchmark test files
│   ├── bench_small_10kb.json
│   ├── bench_huge_50k_items.json
│   ├── bench_deep_500.json
│   └── ...
│
├── scripts/             # Utility scripts
│   ├── generate_benchmarks.py
│   ├── generate_deep_nesting.py
│   └── server.py        # Flask API wrapper
│
├── docs/                # Documentation
│   ├── README.md        # Detailed documentation
│   ├── BENCHMARKING_GUIDE.md
│   └── IMPROVEMENTS_SUMMARY.md
│
├── build.bat            # Windows build script
├── compare_performance.bat
├── Dockerfile
└── requirements.txt
```

## 🚀 Quick Start

### Build Everything
```bat
build.bat
```

This compiles:
- `json_parser.exe` - Main CLI tool
- `test_suite.exe` - Compliance tests
- `benchmark.exe` - Performance testing

### Run Tests
```bat
test_suite.exe
```

### Run Benchmarks
```bat
benchmark.exe benchmarks\bench_*.json
```

### Use Parser
```bat
json_parser.exe validate tests\test.json
json_parser.exe pretty tests\test.json
json_parser.exe get tests\test.json profile.name
```

## 💻 CLI Commands

| Command | Description |
|---------|-------------|
| `validate <file>` | Validate JSON syntax |
| `pretty <file>` | Format with indentation |
| `minify <file>` | Compact JSON |
| `show <file>` | Print parsed tree |
| `get <file> <path>` | Extract value at path |
| `set <file> <path> <value>` | Update value at path |

Add `stdout` as final argument to print instead of writing to file.

## 📊 Performance Metrics

### Throughput
- **Peak**: 70.17 MB/s (string-heavy workload)
- **Average**: 9.42 MB/s (mixed workload)
- **Large files**: 8.67 MB/s (3MB file)

### Memory Efficiency
- **Best case**: 1.3x overhead (string-heavy)
- **Typical**: 6-9x overhead (object-heavy)
- **Deep nesting**: 12.3x overhead (500 levels)

### Robustness
- **Max nesting depth**: 500 levels
- **Test coverage**: 57 ECMA-404 tests
- **Data processed**: 3.59 MB across diverse structures

## 🧪 Testing

### Compliance Tests
```bat
test_suite.exe
```

Tests cover:
- All JSON primitive types
- String escape sequences
- Nested objects and arrays
- Deep nesting (500+ levels)
- Whitespace handling
- Round-trip parsing
- Error detection

### Generate Benchmark Files
```bat
python scripts\generate_benchmarks.py
python scripts\generate_deep_nesting.py
```

### Run Benchmarks
```bat
benchmark.exe benchmarks\bench_*.json
```

## 🌐 Flask API

Start the HTTP server:
```bash
python scripts\server.py
```

Endpoints:
- `POST /validate` - Validate JSON
- `POST /pretty` - Format JSON
- `POST /minify` - Minify JSON
- `POST /show` - Debug print
- `POST /get` - Extract value by path
- `POST /set` - Update value by path

## 🐳 Docker

```bash
docker build -t json-parser-api .
docker run --rm -p 5000:5000 json-parser-api
```

## 🔧 Build Requirements

- **Windows**: MinGW/MSYS2 with g++ (C++17 support)
- **Linux**: g++ 7+ or clang++ 5+
- **Python**: 3.6+ (for scripts and Flask API)

## 📚 Documentation

- **[Full Documentation](docs/README.md)** - Complete API reference
- **[Benchmarking Guide](docs/BENCHMARKING_GUIDE.md)** - Performance testing
- **[Improvements Summary](docs/IMPROVEMENTS_SUMMARY.md)** - Optimization details

## 🎓 Architecture

### Pipeline
```
Input JSON → Tokenizer → Parser → JSONValue AST → Serializer/Navigator
```

### Key Components
- **Tokenizer**: Lexical analysis with line/column tracking
- **Parser**: Recursive descent parser
- **JSONValue**: `std::variant`-based AST
- **Serializer**: Pretty and compact output
- **Navigator**: Path-based access and mutation

## 🚨 Error Handling

- Parser throws `JSONParseError` with line/column
- Tokenizer validates escape sequences
- Path navigator validates structure
- CLI prints contextual error messages with caret


## 📄 License

MIT License - Free for learning and projects

## 🤝 Contributing

This is a learning project. Suggestions and improvements welcome!

---

**Built with**: C++17, Flask, Docker  
**Optimized with**: -O3 -march=native -flto, std::from_chars  
**Tested on**: Windows 10/11, Ubuntu 22.04 (Docker)
