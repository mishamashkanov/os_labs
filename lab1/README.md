# Lab 1 — Process Creation (Windows API)

**Subject:** Operating Systems   
**Topic:** Creating processes using the Windows API

---

## Project Structure

```
lab1/
├── CMakeLists.txt          # CMake build script
├── include/
│   ├── employee.h          # Shared Employee struct (single source of truth)
│   └── win_error.h         # Windows error handling helpers
├── Creator/
│   └── Creator.cpp         # Utility: writes binary file of Employee records
├── Reporter/
│   └── Reporter.cpp        # Utility: reads binary file, produces sorted text report
└── Main/
    └── Main.cpp            # Orchestrator: launches Creator and Reporter via CreateProcess
```

---

## Building

### Requirements
- Windows (any modern version)
- CMake ≥ 3.16
- Visual Studio 2019/2022 **or** MinGW-w64 (GCC for Windows)

### With Visual Studio (MSVC)

```bat
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

Executables will be placed in `build\bin\Release\`.

### With MinGW-w64

```bat
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Executables will be placed in `build\bin\`.

---

## Running

All three executables (`Main.exe`, `Creator.exe`, `Reporter.exe`) **must be in the same directory**. Navigate there and run:

```bat
cd build\bin\Release
Main.exe
```

**Main** will interactively guide you through:
1. Enter a binary file name (e.g. `employees.bin`)
2. Enter the number of employee records to create
3. For each employee, enter: ID, name, hours worked
4. Binary file contents are displayed
5. Enter a report file name (e.g. `report.txt`)
6. Enter the hourly pay rate
7. Report is generated and displayed

### Running Creator and Reporter directly

```bat
# Create a binary file with 3 records
Creator.exe employees.bin 3

# Generate a report with hourly rate 25.50
Reporter.exe employees.bin report.txt 25.50
```

---

## Key Design Decisions

| Requirement | Implementation |
|---|---|
| `WaitForSingleObject(hHandle, INFINITE)` | Used in `LaunchAndWait()` in Main.cpp |
| Shared `Employee` struct | Defined once in `include/employee.h` |
| Error handling | `try/catch` throughout; `win_error.h` wraps `GetLastError()` |
| No magic numbers | All constants defined as `static const` |
| Resource cleanup order | `ScopedHandle` RAII wrapper — handle released in reverse acquisition order |
| No `using namespace std` | All standard library names are fully qualified |
| C++ casts | `reinterpret_cast<>` used instead of C-style casts |
| Struct initialization | `std::memset(&emp, 0, sizeof(Employee))` before use |
| Duplicate file detection | Warning printed if output file already exists |
| Disk full detection | `ofstream` error state checked after each write |

---

## Error Handling Scenarios

- Binary file name is empty → error message, EXIT_FAILURE
- Record count out of range or non-integer → retry loop / error
- File already exists → warning, then overwrite
- `CreateProcess` fails → `GetLastError()` message thrown
- Child process exits with non-zero code → error reported in Main
- Disk full during write → runtime_error thrown
- Negative hours or pay rate → validation error
