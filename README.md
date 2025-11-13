# 🧩 JSON Parser in C++

### 📖 Overview
A fully functional **JSON parser built from scratch in modern C++**, capable of:

- Tokenizing raw JSON text  
- Parsing it using a recursive-descent parser  
- Building a rich JSON tree (`JSONValue`) using `std::variant`  
- Pretty-printing JSON  
- Modifying JSON dynamically using operator overloading  
- Re-serializing JSON back into formatted text  

This project demonstrates how real JSON libraries (like RapidJSON or nlohmann/json) work internally.

---

## ⚙️ Features

- ✅ **Tokenization** – Converts raw JSON text into a sequence of tokens  
- ✅ **Parsing** – Builds a nested JSON tree following JSON grammar  
- ✅ **Data Model (`JSONValue`)** – Holds strings, numbers, booleans, nulls, arrays, objects  
- ✅ **Pretty Printer** – Outputs formatted, human-readable JSON  
- ✅ **Serialization** – Converts the JSON tree back into JSON text  


---

## 🧠 Architecture

The JSON processing pipeline in this project follows four clear stages:

### 1. Raw JSON Text  
You start with a plain JSON string:
- Characters like `{`, `}`, `[`, `]`, `"`, `:`, `,`
- Numbers, booleans, null, etc.

---

### 2. Tokenizer  
The tokenizer scans the raw text and converts it into a sequence of tokens:
- Structural tokens: `{`, `}`, `[`, `]`, `:`, `,`
- STRING tokens
- NUMBER tokens
- TRUE / FALSE / NULL tokens

**Output:** `vector<Token>`

---

### 3. Parser  
A recursive-descent parser processes the token stream and builds a hierarchical JSON tree.

The tree consists of:
- Objects (unordered_map<string, JSONValue>)
- Arrays (vector<JSONValue>)
- Strings
- Numbers
- Booleans
- Null

**Output:** `JSONValue root` (a complete JSON tree)

---

### 4. JSON Tree (In-Memory Representation)  
You can now interact with JSON like native C++ objects:
- `root["name"]`
- `root["marks"][1]`
- `root["profile"]["year"]`

You can also modify the tree:
- change values  
- insert nodes  
- update arrays  

---
### 5. Serializer  
The serializer walks the JSON tree and converts it back into human-readable JSON text.

---

## 📚 Grammar Used
JSON        → VALUE

VALUE       → OBJECT | ARRAY | STRING | NUMBER | TRUE | FALSE | NULL

OBJECT      → { MEMBERS }

MEMBERS     → PAIR | PAIR , MEMBERS

PAIR        → STRING : VALUE

ARRAY       → [ ELEMENTS ]

ELEMENTS    → VALUE | VALUE , ELEMENTS


---

## 🚀 How to Clone, Build & Run

### 1️⃣ Clone the repository
Replace the URL with your repository if different.

    git clone https://github.com/your-username/your-repo-name.git
    cd your-repo-name

---

### 2️⃣ Build the project
Make sure you have a C++17-capable compiler (g++ or clang++). From the project root run:

    g++ -std=c++17 src/*.cpp -Iinclude -o jsonparser.exe

*If you prefer a different output name / OS:*
- Linux/macOS: `./jsonparser.exe`
- Windows (PowerShell/CMD): `jsonparser.exe`

---

### 3️⃣ Run the program
By default `main.cpp` contains a test JSON string. To run:

    jsonparser.exe

You should see pretty-printed JSON output in the terminal.

---
## 🧠 Concepts Implemented

- Recursive-descent parsing  
- Token stream processing  
- JSON DOM (tree) construction  
- Operator overloading for intuitive access  
- Type-safe variant-based JSON storage  
- Smart pointer based memory management  
- Pretty-printing of nested structures  
- Serialization (tree -> JSON string)

---

## 🧰 Technologies Used

- Language: C++17  
- Compiler: g++, Clang++, or MinGW  
- Paradigms: OOP + Functional + Recursive Parsing  
- STL Components: variant, vector, unordered_map, shared_ptr

---

## 🏁 Summary

A complete JSON engine built from scratch in modern C++:

- Tokenizer  
- Parser  
- JSON Tree (DOM)  
- Pretty Printer  
- Serializer  
- Interactive Access & Modification  

This project demonstrates how JSON processing works internally —  
a great systems-level learning project and portfolio addition.




