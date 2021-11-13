# hm3

A *vapourware* minimalist statically typed 'glue language' that can be embedded in C/C++/Rust projects. The language aims to capture the essence of the [lua](https://www.lua.org) and [janet](https://janet-lang.org) runtimes and combine it with a language that is inspired by [standard ML](https://en.wikipedia.org/wiki/Standard_ML).

The project is for fun and learning, though help is wanted if you are able.

The project name is just a placeholder for now.

## Planned Features

- Distributable as a single C file (like the sqlite3 amalgamation).
- Easy to extend from C/C++.
- Easy to embed in C/C++ programs.
- Portable with a small footprint.
- Interactive repl.
- Type inference.
- Compile time code execution.
- Distribute statically linked programs (Bundled VM + bytecode + extensions).
- Deterministic freeing of resources (like python).
- Safe to run untrusted code in a sandbox.

## References

- [crafting interpreters book](https://craftinginterpreters.com/)
