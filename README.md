# 🧩 JSON Parser in C++

### 📖 Overview

A fully functional **JSON parser built from scratch in C++**, capable of tokenizing, parsing, building a JSON tree, and pretty-printing or modifying JSON data.
Implements a **recursive-descent parser** based on JSON grammar rules and uses modern C++ features such as `std::variant`, `std::shared_ptr`, and operator overloading.

---

## ⚙️ Features

✅ **Tokenization** – Converts raw JSON text into a sequence of tokens (`{`, `[`, `,`, `:`, strings, numbers, etc.)
✅ **Parsing** – Follows formal grammar rules to build a nested JSON tree
✅ **Data Model (`JSONValue`)** – Holds strings, numbers, booleans, nulls, arrays, and objects
✅ **Pretty Printer** – Outputs formatted, human-readable JSON
✅ **Interactive Access** – Use `root["key"]` or `root["array"][i]` to read or modify data
✅ **Modern C++ Design** – Uses RAII, smart pointers, and `std::variant` for safety and clarity

---

## 🧠 Architecture

```
json_parser/
├── include/
│   ├── Token.h          # Token definitions
│   ├── tokenizer.h      # Tokenizer class
│   ├── parser.h         # Parser + JSONValue class
│   ├── JSONPrinter.h    # Pretty printing utility
├── src/
│   ├── parser.cpp
│   ├── JSONPrinter.cpp
│   └── main.cpp
└── README.md
```

---

## 📚 Grammar Used

```
JSON        → VALUE
VALUE       → OBJECT | ARRAY | STRING | NUMBER | TRUE | FALSE | NULL
OBJECT      → { MEMBERS }
MEMBERS     → PAIR | PAIR , MEMBERS
PAIR        → STRING : VALUE
ARRAY       → [ ELEMENTS ]
ELEMENTS    → VALUE | VALUE , ELEMENTS
```

---

## 🚀 How to Build & Run

```bash
g++ src/*.cpp -o main.exe
.\main.exe
```

### Example Output

```
{
  "profile": {
    "college": "SRCC",
    "year": 3
  },
  "marks": [98, 99, 93],
  "nickname": null,
  "height": -5.8,
  "student": true,
  "age": 21,
  "name": "Ranveer"
}
```

---

## 🧩 Usage Example (Interactive Access)

```cpp
cout << root["name"].asString();             // "Ranveer"
cout << root["profile"]["college"].asString(); // "SRCC"
root["age"] = JSONValue(21.0);
root["marks"][1] = JSONValue(99.0);
```

---

## 🧠 Concepts Implemented

* Recursive-descent parsing
* Token stream processing
* Abstract syntax tree (DOM-style structure)
* Operator overloading
* Type-safe access via `std::variant`

---

## 🧰 Technologies

* **Language:** C++17
* **Compiler:** g++ / clang++
* **Paradigm:** Object-oriented + functional elements

---

## 🧑‍💻 Author

**Ranveer Verma**
Curious learner passionate about systems programming and data engineering.

> “I built this project to understand how JSON parsers work under the hood — from tokenization to syntax validation to data manipulation.”

---

## 🌟 Future Enhancements

* [ ] JSON serialization (`toJSONString()`)
* [ ] File read/write support
* [ ] CLI tool for pretty-printing JSON files
* [ ] Minify / Beautify modes

---

## 🏁 Quick Summary

> Built a complete JSON parser from scratch using modern C++.
> Supports reading, parsing, accessing, and modifying JSON structures with a custom data model.


