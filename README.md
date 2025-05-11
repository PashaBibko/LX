# LX Language

This is my custom compiled language written in C++ based off of the LLVM toolchain. Do not use the language in it's current state unless you are insane.

## Build

Requires VS-22 with C++ and C# development to be downloaded. Run LX-Compiler.sln and run the project. Currently it defaults to using the source file example/main.ll but that can be modified in Main.cs

## Syntax

#### Comments

```
# Comments start and end with hashtags #
```

#### Variables

```
int a # Declares an unitialised integer (currently the only type) #
int b = 5 # Declares integer with a value of 5 #
```

#### Operations
```
# Currently only the basic maths operations are implemented #
int c = 1 + 2 - 3 / 4 * 5
```

#### Functions
```
# All functions return int #
func add(int a, int b)
{
    return a + b
}
```

## Features

### Planned features (in order)
- More than just int as a type (float, uint, double, char)
- References and pointers
- Structs / Classes (Polymorphism + vtables)
- String and string manipulation (needs classes)

### Codebase
- New features

### Stuff I want to do later (unordered)
- I/O manager (Console, Files)
- Debugger support
- Extensions for vscode / vs22
- Build system
- SSA (Static single assignment)
- LX-Core (Basic Standard Lib)
- SIMD (Single instruction, multiple data)
- Built in features for multi-language codebases (C, C++, C#, Rust...)
- Custom Linker written in rust
- Package manager
- Custom optimiser (as well O3 in Clang)
- Complicated LX-Core:
    - Graphics APIs support
    - Networking support
    - WinAPI compatiability

### Stuff I might do later (but probably won't)
- JIT (Just in time compiler) / Interpreter
- Different compiler backends
- Python support
- Non-windows OS support
- Database support
