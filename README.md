# 🧠 Memory Debugger - Custom Memory Debugging Utility for C

`Memory Debugger` is a lightweight and extensible C library for detecting and reporting common memory issues such as:

- Memory leaks
- Double frees
- Buffer overflows
- Memory usage statistics
- Guarded allocations for overwrite detection

It provides detailed runtime information and optional file logging to help C developers debug memory issues in real-time.

---

## 📁 Project Structure

```plaintext

├── main.c               # Example usage of the memdebug utility
├── memdebug.h           # Header file defining the memdebug API
├── memdebug.c           # Implementation of the custom memory debugger
├── CMakeLists.txt       # CMake configuration for easy building
└── memdebug_report.log  # (Generated at runtime if file logging is enabled)

