# 🧠 Memory Debugger - Custom Memory Debugging Utility for C

`Memory Debugger` is a lightweight and extensible C library for detecting and reporting common memory issues such as:

- Memory leaks;
- Double frees;
- Buffer overflows;
- Memory usage statistics;
- Guarded allocations for overwrite detection.

It provides detailed runtime information and optional file logging to help C developers debug memory issues in real-time.

---

## 📁 Project Structure

```plaintext

├── main.c               # Example usage of the memdebug utility
├── memdebug.h           # Header file defining the memdebug API
├── memdebug.c           # Implementation of the custom memory debugger
├── CMakeLists.txt       # CMake configuration for easy building
└── memdebug_report.log  # (Generated at runtime if file logging is enabled)

```
# 🛠️ Features

- 🧠 Tracks malloc, calloc, realloc, and free;
- 🛑 Detects double free errors;
- 🕳️ Detects memory leaks;
- 🧱 Uses guard bytes to detect buffer overruns;
- 🧾 Logs detailed allocation info (file and line number);
- 🌐 Supports multithreaded access (using mutexes);
- 📄 Optional file logging (to memdebug_report.log);
- 🇧🇷 / 🇺🇸 Language support for output (Portuguese or English);
- 🧪 Runtime verbosity control: QUIET, NORMAL, DEBUG.

# 🚀 Getting Started

✅ Requirements

- A C compiler (gcc, clang, MSVC, etc.);
- CLion IDE (optional but recommended);
- CMake 3.10 or higher.

# 🏗️ Building with CMake

🔧 Using CLion (Recommended)

1. Open the project directory in CLion;
2. CLion will auto-detect the CMakeLists.txt file;
3. Click Run to compile and execute main.c

# 💻 Manual CMake Build (Command Line)

```plaintext

# Clone the repo
git clone https://github.com/0xRhayanne/MemoryDebugger.git
cd memdebug

# Create build directory
mkdir build && cd build

# Generate build files and compile
cmake ..
cmake --build .

# Run the executable
./MemoryDebugger

```
# 📄 CMakeLists.txt

```plaintext

cmake_minimum_required(VERSION 3.10)
project(MemoryDebugger C)

set(CMAKE_C_STANDARD 11)

add_executable(MemoryDebugger
    main.c
    memdebug.c
)

```
# 🔍 How It Works

This utility overrides standard memory functions:

```plaintext

#define malloc(sz)      md_malloc(sz, __FILE__, __LINE__)
#define calloc(n, sz)   md_calloc(n, sz, __FILE__, __LINE__)
#define realloc(p, sz)  md_realloc(p, sz, __FILE__, __LINE__)
#define free(p)         md_free(p, __FILE__, __LINE__)

```

It tracks every allocation, adds guard bytes before and after the allocated memory, and checks for any corruption or misuse.

At the end of your program, call:

```plaintext

md_report();

```
