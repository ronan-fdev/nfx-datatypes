# Benchmarks

---

## Test Environment

### Hardware Configuration

| Component                | Specification                                                 |
| ------------------------ | ------------------------------------------------------------- |
| **CPU**                  | 12th Gen Intel Core i7-12800H (20 logical, 14 physical cores) |
| **Base Clock**           | 2.80 GHz                                                      |
| **Turbo Clock**          | 4.80 GHz                                                      |
| **L1 Data Cache**        | 48 KiB (×6 P-cores) + 32 KiB (×8 E-cores)                     |
| **L1 Instruction Cache** | 32 KiB (×6 P-cores) + 64 KiB (×2 E-core clusters)             |
| **L2 Unified Cache**     | 1.25 MiB (×6 P-cores) + 2 MiB (×2 E-core clusters)            |
| **L3 Unified Cache**     | 24 MiB (×1 shared)                                            |
| **RAM**                  | DDR4-3200 (32GB)                                              |
| **GPU**                  | NVIDIA RTX A2000 4GB GDDR6                                    |

### Software Configuration

| Platform    | Benchmark Framework     | C++ Compiler              | nfx-datatypes Version |
| ----------- | ----------------------- | ------------------------- | --------------------- |
| **Linux**   | Google Benchmark v1.9.4 | GCC 14.2.0-x64            | v1.0.0                |
| **Linux**   | Google Benchmark v1.9.4 | Clang 19.1.7-x64          | v1.0.0                |
| **Windows** | Google Benchmark v1.9.4 | MinGW GCC 14.2.0-x64      | v1.0.0                |
| **Windows** | Google Benchmark v1.9.4 | Clang-GNU-CLI 19.1.5-x64  | v1.0.0                |
| **Windows** | Google Benchmark v1.9.4 | Clang-MSVC-CLI 19.1.5-x64 | v1.0.0                |
| **Windows** | Google Benchmark v1.9.4 | MSVC 19.44.35217.0-x64    | v1.0.0                |

---

# Performance Results

## Decimal Operations

### Construction Benchmarks

| Operation                 | Linux GCC    | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------- | ------------ | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| **Construct Default**     | **0.209 ns** | 0.212 ns    | 0.215 ns          | 0.234 ns              | 0.220 ns               | 0.858 ns     |
| **Construct from Int32**  | **0.460 ns** | 0.467 ns    | 0.715 ns          | 0.594 ns              | 0.531 ns               | 1.37 ns      |
| **Construct from Int64**  | **0.459 ns** | 0.465 ns    | 0.516 ns          | 0.578 ns              | 0.516 ns               | 1.34 ns      |
| **Construct from Uint32** | **0.459 ns** | 0.474 ns    | 0.698 ns          | 0.558 ns              | 0.544 ns               | 1.35 ns      |
| **Construct from Uint64** | **0.572 ns** | 0.697 ns    | 0.575 ns          | 0.785 ns              | 0.732 ns               | 1.37 ns      |
| **Construct from Float**  | 67.7 ns      | **66.3 ns** | 162 ns            | 82.3 ns               | 82.3 ns                | 78.8 ns      |
| **Construct from Double** | 62.7 ns      | **60.8 ns** | 172 ns            | 76.0 ns               | 77.2 ns                | 75.9 ns      |
| **Construct from Int128** | 2.60 ns      | **2.42 ns** | 2.25 ns           | 3.07 ns               | 3.14 ns                | 4.20 ns      |
| **Copy Construct**        | **0.216 ns** | 0.217 ns    | 0.230 ns          | 0.246 ns              | 0.235 ns               | 0.767 ns     |

### Arithmetic Operations

| Operation                     | Linux GCC   | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------- | ----------- | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **Addition**                  | 15.3 ns     | 12.6 ns      | **11.2 ns**       | 26.2 ns               | 26.2 ns                | 26.7 ns      |
| **Subtraction**               | 17.7 ns     | 16.9 ns      | **11.4 ns**       | 36.8 ns               | 36.1 ns                | 37.5 ns      |
| **Multiplication**            | 59.2 ns     | **54.7 ns**  | 174 ns            | 150 ns                | 151 ns                 | 153 ns       |
| **Multiplication Large**      | **60.2 ns** | 62.8 ns      | 214 ns            | 173 ns                | 169 ns                 | 176 ns       |
| **Division**                  | **44.6 ns** | 46.9 ns      | 171 ns            | 220 ns                | 215 ns                 | 246 ns       |
| **Division High Precision**   | **41.8 ns** | 45.8 ns      | 174 ns            | 172 ns                | 165 ns                 | 184 ns       |
| **Unary Minus**               | 0.815 ns    | **0.469 ns** | 0.753 ns          | 0.562 ns              | 0.530 ns               | 1.07 ns      |
| **Addition Assignment**       | 15.2 ns     | 15.3 ns      | **14.8 ns**       | 33.7 ns               | 32.2 ns                | 33.1 ns      |
| **Subtraction Assignment**    | 17.8 ns     | 17.4 ns      | **14.1 ns**       | 38.5 ns               | 36.1 ns                | 37.5 ns      |
| **Multiplication Assignment** | **58.0 ns** | 63.4 ns      | 90.0 ns           | 165 ns                | 164 ns                 | 153 ns       |
| **Division Assignment**       | **44.9 ns** | 51.4 ns      | 46.0 ns           | 225 ns                | 218 ns                 | 246 ns       |

### Parsing Operations

| Operation                | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------ | --------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| **Parse Integer**        | 20.4 ns   | 23.5 ns     | **13.8 ns**       | 68.0 ns               | 64.2 ns                | 71.1 ns      |
| **Parse Small Decimal**  | 26.5 ns   | 27.5 ns     | **19.5 ns**       | 81.6 ns               | 82.0 ns                | 83.7 ns      |
| **Parse Large Decimal**  | 105 ns    | 118 ns      | **67.0 ns**       | 361 ns                | 322 ns                 | 361 ns       |
| **Parse High Precision** | 108 ns    | **100 ns**  | 61.4 ns           | 353 ns                | 337 ns                 | 408 ns       |
| **Parse Negative**       | 75.5 ns   | 80.5 ns     | **38.4 ns**       | 225 ns                | 209 ns                 | 223 ns       |
| **TryParse Valid**       | 33.6 ns   | 41.5 ns     | **23.5 ns**       | 109 ns                | 109 ns                 | 117 ns       |
| **TryParse Invalid**     | 5.65 ns   | **5.38 ns** | 5.47 ns           | 6.42 ns               | 6.28 ns                | 14.3 ns      |

### Conversion & Formatting

| Operation             | Linux GCC | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| --------------------- | --------- | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **ToDouble**          | 14.5 ns   | **12.8 ns**  | 63.5 ns           | 13.5 ns               | 13.2 ns                | 14.8 ns      |
| **ToBits**            | 0.707 ns  | **0.488 ns** | **0.488 ns**      | 0.531 ns              | 0.516 ns               | 1.46 ns      |
| **ToString Integer**  | 23.1 ns   | **22.1 ns**  | 42.4 ns           | 58.6 ns               | 56.2 ns                | 55.8 ns      |
| **ToString Small**    | 38.1 ns   | **36.6 ns**  | 60.0 ns           | 85.8 ns               | 92.1 ns                | 85.8 ns      |
| **ToString Large**    | 36.7 ns   | **36.3 ns**  | 62.8 ns           | 81.6 ns               | 81.6 ns                | 80.2 ns      |
| **ToString Negative** | 234 ns    | **205 ns**   | 451 ns            | 229 ns                | 210 ns                 | 220 ns       |

### Comparison Operations

| Operation               | Linux GCC  | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------- | ---------- | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **Equality Decimal**    | 7.97 ns    | 6.78 ns      | **2.62 ns**       | 8.89 ns               | 8.54 ns                | 8.37 ns      |
| **LessThan Decimal**    | 8.28 ns    | **2.91 ns**  | 3.99 ns           | 8.72 ns               | 8.58 ns                | 8.58 ns      |
| **GreaterThan Decimal** | 17.7 ns    | 9.92 ns      | **7.32 ns**       | 19.5 ns               | 17.2 ns                | 18.4 ns      |
| **Equality Int32**      | 8.87 ns    | 7.45 ns      | **1.69 ns**       | 9.42 ns               | 9.03 ns                | 9.42 ns      |
| **Equality Int64**      | 8.94 ns    | 7.70 ns      | **1.95 ns**       | 9.42 ns               | 9.21 ns                | 9.07 ns      |
| **Equality Uint64**     | 9.44 ns    | 10.6 ns      | **1.95 ns**       | 11.2 ns               | 11.2 ns                | 9.42 ns      |
| **Equality Double**     | **578 ns** | 551 ns       | 2.20 μs           | 1.17 μs               | 1.15 μs                | 1.27 μs      |
| **LessThan Double**     | **518 ns** | 510 ns       | 2.19 μs           | 1.23 μs               | 1.15 μs                | 1.20 μs      |
| **Equality Float**      | **508 ns** | 497 ns       | 1.18 μs           | 1.17 μs               | 1.03 μs                | 1.15 μs      |
| **Equality Int128**     | 2.37 ns    | **0.820 ns** | 1.76 ns           | 1.93 ns               | 2.09 ns                | 3.01 ns      |
| **LessThan Int128**     | 2.90 ns    | **1.46 ns**  | 2.10 ns           | 2.40 ns               | 2.61 ns                | 3.14 ns      |

### State Checking & Mathematical Operations

| Operation                 | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------- | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **IsZero (Zero)**         | **0.106 ns** | 0.215 ns     | 0.107 ns          | 0.225 ns              | 0.225 ns               | 0.558 ns     |
| **IsZero (NonZero)**      | 0.235 ns     | **0.229 ns** | 0.251 ns          | 0.246 ns              | 0.298 ns               | 0.767 ns     |
| **IsNegative (Negative)** | **0.350 ns** | 0.465 ns     | 0.392 ns          | 0.547 ns              | 0.497 ns               | 1.05 ns      |
| **IsNegative (Positive)** | **0.349 ns** | 0.466 ns     | 0.377 ns          | 0.562 ns              | 0.500 ns               | 1.00 ns      |
| **Abs Positive**          | **0.479 ns** | 0.480 ns     | 0.572 ns          | 0.547 ns              | 0.531 ns               | 1.05 ns      |
| **Abs Negative**          | 1.32 ns      | 0.720 ns     | **0.715 ns**      | 0.879 ns              | 0.795 ns               | 1.38 ns      |
| **Truncate**              | 121 ns       | **111 ns**   | 460 ns            | 377 ns                | 361 ns                 | 344 ns       |
| **Floor**                 | 118 ns       | **110 ns**   | 464 ns            | 384 ns                | 369 ns                 | 368 ns       |
| **Ceiling**               | **112 ns**   | 115 ns       | 481 ns            | 392 ns                | 375 ns                 | 366 ns       |
| **Round**                 | 122 ns       | **111 ns**   | 475 ns            | 368 ns                | 353 ns                 | 337 ns       |

### Property Accessors & Constants

| Operation              | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------------- | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **GetScale**           | **0.233 ns** | 0.464 ns     | 0.246 ns          | 0.516 ns              | 0.502 ns               | 1.00 ns      |
| **DecimalPlacesCount** | 5.61 ns      | **3.52 ns**  | 5.47 ns           | 4.14 ns               | 3.93 ns                | 4.05 ns      |
| **Constant Zero**      | 0.212 ns     | **0.210 ns** | 0.213 ns          | 0.244 ns              | 0.218 ns               | 0.732 ns     |
| **Constant One**       | **0.212 ns** | 0.325 ns     | 0.222 ns          | 0.229 ns              | 0.229 ns               | 0.942 ns     |
| **Constant MinValue**  | **0.354 ns** | 0.391 ns     | 0.488 ns          | 0.368 ns              | 0.500 ns               | 1.20 ns      |
| **Constant MaxValue**  | **0.390 ns** | 0.383 ns     | 0.500 ns          | 0.368 ns              | 0.488 ns               | 1.17 ns      |

---

## Int128 Operations

### Construction Benchmarks

| Operation                   | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| --------------------------- | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **Construct Default**       | **0.210 ns** | 0.211 ns     | 0.215 ns          | 0.215 ns              | 0.213 ns               | 0.698 ns     |
| **Construct from Int32**    | **0.209 ns** | 0.211 ns     | 0.213 ns          | 0.236 ns              | 0.213 ns               | 0.698 ns     |
| **Construct from Int64**    | **0.211 ns** | 0.273 ns     | 0.218 ns          | 0.225 ns              | 0.213 ns               | 0.711 ns     |
| **Construct from Uint64**   | **0.210 ns** | **0.210 ns** | 0.215 ns          | 0.265 ns              | 0.213 ns               | 0.698 ns     |
| **Construct from TwoWords** | **0.427 ns** | 0.454 ns     | 0.476 ns          | 0.530 ns              | 0.544 ns               | 0.963 ns     |
| **Construct from Float**    | 1.70 ns      | **1.38 ns**  | 1.92 ns           | 7.67 ns               | 7.50 ns                | 9.42 ns      |
| **Construct from Double**   | 358 ns       | **339 ns**   | 547 ns            | 732 ns                | 750 ns                 | 785 ns       |
| **Construct from Decimal**  | 6.40 ns      | 2.33 ns      | **2.01 ns**       | 2.25 ns               | 2.46 ns                | 6.56 ns      |
| **Copy Construct**          | **0.210 ns** | 0.213 ns     | 0.225 ns          | 0.230 ns              | 0.225 ns               | 0.715 ns     |

### Arithmetic Operations

| Operation                | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------ | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **Addition**             | 0.462 ns     | 0.505 ns     | **0.408 ns**      | 0.586 ns              | 0.586 ns               | 0.952 ns     |
| **Subtraction**          | 0.463 ns     | 0.504 ns     | **0.408 ns**      | 0.594 ns              | 0.572 ns               | 1.23 ns      |
| **Multiplication**       | **0.692 ns** | 0.693 ns     | 0.880 ns          | 2.09 ns               | 2.09 ns                | 2.73 ns      |
| **Multiplication Large** | 0.694 ns     | **0.693 ns** | 0.890 ns          | 2.04 ns               | 2.09 ns                | 2.68 ns      |
| **Division**             | 4.31 ns      | **4.30 ns**  | 5.44 ns           | 5.94 ns               | 5.86 ns                | 6.42 ns      |
| **Division Large**       | **3.54 ns**  | 3.68 ns      | 6.56 ns           | 143 ns                | 131 ns                 | 222 ns       |
| **Modulo**               | 4.38 ns      | **4.24 ns**  | 5.78 ns           | 7.67 ns               | 7.67 ns                | 7.50 ns      |
| **Unary Minus**          | **0.275 ns** | 0.311 ns     | 0.318 ns          | 0.977 ns              | 1.40 ns                | 1.73 ns      |

### Parsing Operations

| Operation                 | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------- | --------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| **Parse Small Number**    | 5.43 ns   | **2.80 ns** | 3.38 ns           | 12.0 ns               | 11.4 ns                | 13.7 ns      |
| **Parse Medium Number**   | 57.1 ns   | **13.5 ns** | 19.7 ns           | 40.8 ns               | 39.0 ns                | 44.4 ns      |
| **Parse Large Number**    | 94.6 ns   | **32.9 ns** | 50.2 ns           | 76.7 ns               | 75.0 ns                | 87.9 ns      |
| **Parse Negative Number** | 66.7 ns   | **23.9 ns** | 34.5 ns           | 60.0 ns               | 61.4 ns                | 66.3 ns      |
| **TryParse Valid**        | 67.2 ns   | **41.7 ns** | 54.7 ns           | 51.6 ns               | 50.0 ns                | 59.4 ns      |
| **TryParse Invalid**      | 1.63 ns   | **1.41 ns** | 1.73 ns           | 2.46 ns               | 2.20 ns                | 2.73 ns      |

### Conversion & Formatting

| Operation             | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| --------------------- | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **ToLow**             | 0.227 ns     | 0.336 ns     | **0.105 ns**      | 0.481 ns              | 0.471 ns               | 0.942 ns     |
| **ToHigh**            | **0.226 ns** | 0.318 ns     | 0.241 ns          | 0.463 ns              | 0.500 ns               | 0.942 ns     |
| **ToBits**            | 1.03 ns      | **0.459 ns** | 0.984 ns          | 0.739 ns              | 0.711 ns               | 1.44 ns      |
| **ToString Small**    | 31.6 ns      | **28.7 ns**  | 48.7 ns           | 54.7 ns               | 54.7 ns                | 53.1 ns      |
| **ToString Medium**   | 256 ns       | **235 ns**   | 337 ns            | 375 ns                | 353 ns                 | 353 ns       |
| **ToString Large**    | 631 ns       | **615 ns**   | 1109 ns           | 1423 ns               | 1283 ns                | 1245 ns      |
| **ToString Negative** | 644 ns       | **625 ns**   | 1147 ns           | 1535 ns               | 1318 ns                | 1172 ns      |

### Comparison Operations

| Operation              | Linux GCC    | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------------- | ------------ | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| **Equality Int128**    | **0.312 ns** | 0.461 ns    | 0.338 ns          | 0.461 ns              | 0.417 ns               | 0.753 ns     |
| **LessThan Int128**    | **0.321 ns** | 0.471 ns    | 0.361 ns          | 0.562 ns              | 0.500 ns               | 0.732 ns     |
| **GreaterThan Int128** | **0.320 ns** | 0.464 ns    | 0.377 ns          | 0.614 ns              | 0.586 ns               | 0.732 ns     |
| **Equality Int64**     | **0.105 ns** | 0.238 ns    | **0.105 ns**      | 0.254 ns              | 0.209 ns               | 0.698 ns     |
| **LessThan Int64**     | 0.106 ns     | 0.232 ns    | **0.105 ns**      | 0.262 ns              | 0.210 ns               | 0.698 ns     |
| **Equality Uint64**    | **0.105 ns** | 0.248 ns    | **0.105 ns**      | 0.262 ns              | 0.209 ns               | 0.865 ns     |
| **Equality Double**    | 2.59 ns      | 2.96 ns     | **0.105 ns**      | 4.88 ns               | 4.17 ns                | 3.07 ns      |
| **LessThan Double**    | 2.61 ns      | 2.86 ns     | **0.105 ns**      | 3.53 ns               | 3.37 ns                | 2.93 ns      |
| **Equality Float**     | 2.59 ns      | 2.80 ns     | **0.105 ns**      | 4.14 ns               | 4.08 ns                | 3.05 ns      |
| **Equality Decimal**   | 2.10 ns      | 2.21 ns     | 2.09 ns           | 1.80 ns               | **1.72 ns**            | 2.56 ns      |
| **LessThan Decimal**   | **2.11 ns**  | 2.29 ns     | 2.35 ns           | 2.62 ns               | 2.25 ns                | 2.51 ns      |

### State Checking & Mathematical Operations

| Operation                 | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------- | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **IsZero (Zero)**         | **0.105 ns** | 0.227 ns     | **0.105 ns**      | 0.225 ns              | 0.218 ns               | 0.711 ns     |
| **IsZero (NonZero)**      | **0.219 ns** | 0.346 ns     | 0.229 ns          | 0.402 ns              | 0.338 ns               | 0.750 ns     |
| **IsNegative (Negative)** | **0.105 ns** | 0.212 ns     | **0.105 ns**      | 0.258 ns              | 0.218 ns               | 0.698 ns     |
| **IsNegative (Positive)** | **0.105 ns** | 0.210 ns     | **0.105 ns**      | 0.213 ns              | 0.213 ns               | 0.698 ns     |
| **Abs Positive**          | 0.464 ns     | 0.466 ns     | 0.500 ns          | **0.285 ns**          | 0.305 ns               | 0.725 ns     |
| **Abs Negative**          | 0.213 ns     | **0.210 ns** | 0.225 ns          | 1.34 ns               | 1.27 ns                | 1.73 ns      |

---

_Benchmarks executed on October 26, 2025_
