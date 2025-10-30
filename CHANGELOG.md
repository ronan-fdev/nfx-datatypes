# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- NIL

### Changed

- NIL

### Deprecated

- NIL

### Removed

- NIL

### Fixed

- NIL

### Security

- NIL

## [1.0.2] - 2025-10-30

### Changed

- Refactored directory variable naming from `NFX_DATATYPES_ROOT_DIR` to `NFX_DATATYPES_DIR` for consistency

- **GitHub Actions Release Workflow**

  - Removed automatic trigger on release publication to prevent GitHub Pages protection errors
  - Documentation now deploys only via manual trigger or push to main branch

## [1.0.1] - 2025-10-28

### Changed

- **GitHub Actions Release Workflow**

  - Added mandatory tag validation to prevent accidental releases
  - Added semantic versioning format verification (X.Y.Z)
  - Added `create-release` job to create GitHub release first
  - Added job dependencies (`needs: create-release`) for proper release workflow ordering
  - Set all release uploads to `draft: true` for manual review before publishing
  - Removed problematic `github.ref_name` fallback that could create "main" releases
  - Changed `fail_on_unmatched_files` to `false` for better error handling
  - Added GCC-14 installation via PPA for Ubuntu 22.04 compatibility
  - Added package verification steps for all build jobs (Linux, Windows, Source)
  - Added SHA256 checksum generation for all packages
  - Added workflow artifacts upload with 7-day retention for debugging
  - Removed redundant CMake compiler flags (MSVC environment auto-detection)

- **Documentation Configuration**
  - Fixed Doxygen paths to use `NFX_DATATYPES_ROOT_DIR` instead of `CMAKE_SOURCE_DIR` for better modularity
  - Updated paths in `doc/CMakeLists.txt` for README, CHANGELOG, and sample files

### Added

- **Release Workflow Enhancements**
  - Package verification with detailed logging and error reporting
  - SHA256SUMS.txt files for all Linux and Windows packages
  - SHA256SUMS-source.txt for source packages
  - Workflow artifacts for post-build debugging and verification
  - Build validation before release upload

## [1.0.0] - 2025-10-26 - Initial Release

### Added

- **Int128**: Cross-platform 128-bit signed integer arithmetic

  - Native `__int128` support on GCC/Clang for maximum performance
  - Hand-optimized implementation for MSVC using 64-bit operations
  - Full range: -2^127 to 2^127-1
  - Complete arithmetic operators (+, -, \*, /, %, unary -)
  - Comparison operators (==, !=, <, <=, >, >=)
  - Type conversions (int32, int64, uint64, float, double, Decimal)
  - String parsing and formatting (`parse()`, `tryParse()`, `toString()`)
  - State checking methods (`isZero()`, `isNegative()`)
  - Bit manipulation (`toLow()`, `toHigh()`, `toBits()`)

- **Decimal**: High-precision decimal arithmetic

  - 96-bit mantissa with 8-bit scale (0-28 decimal places)
  - Up to 28-29 significant digits
  - Exact decimal representation (no binary floating-point errors)
  - Complete arithmetic operators (+, -, \*, /, unary -)
  - Comparison operators (==, !=, <, <=, >, >=)
  - Type conversions (int32, int64, uint64, float, double, Int128)
  - String parsing and formatting (`parse()`, `tryParse()`, `toString()`)
  - Mathematical operations (`abs()`, `truncate()`, `floor()`, `ceiling()`, `round()`)
  - State checking methods (`isZero()`, `isNegative()`, `scale()`, `decimalPlacesCount()`)
  - Cross-type arithmetic (Decimal ↔ Int128 interoperability)

- **Build System**

  - CMake 3.20+ configuration with modular structure
  - Static and shared library targets
  - Cross-platform support (Linux GCC/Clang, Windows MinGW/Clang/MSVC)
  - Google Test integration for unit testing
  - Google Benchmark integration for performance benchmarking
  - Doxygen documentation generation support
  - CPack packaging for distribution (DEB, RPM, ZIP, WIX)
  - Installation support with CMake package config

- **Documentation**

  - README with feature overview
  - Detailed API documentation with Doxygen comments
  - Sample application demonstrating library usage
  - Build and installation instructions

- **Testing & Benchmarking**
  - Unit test suite
  - Performance benchmarks for all operations
  - Cross-compiler performance validation
