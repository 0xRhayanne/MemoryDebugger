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

This prints a summary of memory usage, leaks, and issues.

# 📷 Example Output

```plaintext

=== RELATÓRIO DE MEMÓRIA ===
┌──────────────┬────────┬─────────────┬──────┐
│ Ponteiro     │ Tamanho│ Arquivo     │ Linha│
├──────────────┼────────┼─────────────┼──────┤
│  0x123abc    │    50  │ main.c      │   18 │
└──────────────┴────────┴─────────────┴──────┘
Total de alocações: 4 | Total de frees: 3 | Total de reallocs: 1
Pico de memória: 80 bytes | Maior alocação: 80 bytes

```
# ⚙️ Settings and Configuration

🌐 Language

Set output language:

```plaintext

md_set_language(PORTUGUESE); // or ENGLISH

```
# 📢 Verbosity

Control debug output:

```plaintext

md_set_verbosity(QUIET);  // No logs
md_set_verbosity(NORMAL); // Default
md_set_verbosity(DEBUG);  // Detailed logs

```

# 📄 File Logging

Enable or disable file logging to memdebug_report.log:

```plaintext

md_enable_file_logging(1); // Enable

```
# 🔒 Thread Safety

```memdebug``` uses platform-specific mutexes (```pthread``` or Windows ```CRITICAL_SECTION```) to ensure thread safety during memory operations.

&nbsp;

# 🐞 Known Limitations & Caveats

- Thread safety currently fully implemented only on Windows (via ```CRITICAL_SECTION```);
- Guard bytes help detect overruns but may not catch every memory corruption;
- Not intended as a full replacement for advanced tools like Valgrind or ASan;
- Language support limited to English and Portuguese.


# 🧩 How to Contribute

- Feel free to open issues or submit pull requests;
- Suggestions for new features or bug fixes are welcome;

&nbsp;

<p align="center">
  <img src="https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExcTg1bmNrbmRwNzkwZDZncDBpZDVwczRuaWRkYTNtNnJyaDg1anlucCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/uWlpPGquhGZNFzY90z/giphy.gif" width="300" alt="Centered GIF" />
</p>

