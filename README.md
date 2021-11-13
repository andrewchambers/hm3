# hm3

A *WIP* minimalist statically typed 'glue language' that can be embedded in C/C++/Rust projects. The language aims to capture the essence of the [lua](https://www.lua.org) and [janet](https://janet-lang.org) runtimes and apply it to a language that resembles [standard ML](https://en.wikipedia.org/wiki/Standard_ML) (with tweaks).

The project is mostly for fun and learning, so do not assume it will be finished, especially
if you do not help.

## Planned Features

- Distributable as a single C file (like the sqlite3 amalgamation).
- Easy to extend from C/C++.
- Easy to embed in C/C++ programs.
- Portable with a small footprint.
- Interactive repl.
- Type inference.
- Compile time code execution.
- Ability to distribute statically linked programs (VM + bytecode + C extensions).
- Deterministic freeing of resources (like python).
- Safe to run untrusted code in a sandbox.

## References

- [crafting interpreters book](https://craftinginterpreters.com/)
