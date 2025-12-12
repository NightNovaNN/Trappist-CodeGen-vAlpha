# 🔭 Trappist — CodeGen Pipeline (Alpha)

Trappist is a new compiled programming language written in C, inspired by the elegance and mystery of the **TRAPPIST-1** exoplanet system.  
This repository contains the **frontend and bytecode generation pipeline** — the core of the Trappist compiler.

This is an **early alpha**, meaning the structure is here, the pipeline runs, but features are minimal and the language is still evolving.

---

## 🚀 Current Pipeline Overview

Trappist compiler currently performs:

1. **Lexing**  
   Converts raw source code → token stream  
   (`lexer/front.c`)

2. **Parsing**  
   Converts tokens → AST / internal representation  
   (`parser/parser.c`, `parser/reader.c`)

3. **Bytecode Emission**  
   AST → low-level Trappist bytecode  
   (`bytecode/bc.c`)

4. **Executable Wrapper**  
   Compiler entry point and build control  
   (`trap.c`)

This completes the **Frontend → Bytecode** stage of the compiler.

---

## 🧪 Project Status (Alpha v0.1)

- Lexer ✔  
- Parser ✔  
- Bytecode generator ✔  
- Codegen pipeline build system ✔  
- IR design WIP  
- Runtime / VM ❌ (coming soon)  
- Standard library ❌  
- Optimizer ❌  
- Full language spec ❌  

This repository is actively evolving — expect rapid changes.

---

## 🛠️ Building the Compiler

### **Requirements**
- GCC or Clang  
- Make  
- Windows / Linux / macOS  

### **Build**

```sh
make
````

If successful, this produces the compiler binary:

```
./trap
```

### **Run**

```sh
./trap <source.trp> <output.byc>
```

---

## 📁 Folder Structure

```
codegen/
│
├── lexer/
│   ├── front.c
│   └── lex.h
│
├── parser/
│   ├── parser.c
│   ├── parser.h
│   ├── reader.c
│   └── reader.h
│
├── bytecode/
│   ├── bc.c
│   └── bc.h
│
├── tests/
│   ├── test.trp
│   └── tok.le
│
├── trap.c
└── Makefile
```

---

## 🌌 Why “Trappist”?

The name is inspired by the **TRAPPIST-1** system — a compact collection of seven planets orbiting a cool red dwarf.
The goal of Trappist (the language) is to be:

* **fast**
* **predictable**
* **low-level**
* **minimal runtime**
* **no hidden magic**
* **macro-friendly**
* **easy to reason about**

A language that feels like a stable cosmic system: small, elegant, and powerful.

---

## 🗺️ Roadmap

### **Milestone 1 — Frontend (DONE)**

* Lexer
* Parser
* Bytecode emission

### **Milestone 2 — Virtual Machine**

* Bytecode interpreter
* Stack engine
* Built-in ops

### **Milestone 3 — Standard Library**

* Strings
* Math
* IO
* Basic memory API

### **Milestone 4 — Language Features**

* Macros / compile-time system
* Structured types
* Functions
* Modules

### **Milestone 5 — Optimizer**

* Control flow analysis
* Constant folding
* Dead code elimination

---

## 🤝 Contributing

This is an early alpha, but feedback, ideas, and feature requests are welcome.
Open issues or PRs if you'd like to help shape the language.

---

## ⭐ License

MIT License — free to use, modify, and distribute.

---

## 🪐 Author

**NightNova** — compiler dev, tech enthusiast, and cosmic explorer.

---

