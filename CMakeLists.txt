#==============================================================================
# nfx-datatypes - Top-level CMake configuration
#==============================================================================

#----------------------------------------------
# Project definition
#----------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(nfx-datatypes
	VERSION 1.0.0
	DESCRIPTION "Cross-platform C++ library with high-precision Int128 and Decimal datatypes"
	HOMEPAGE_URL "https://github.com/ronan-fdev/nfx-datatypes"
	LANGUAGES CXX
)

#----------------------------------------------
# Installation prefix configuration
#----------------------------------------------

# Set default installation path to nfx/datatypes
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	if(WIN32)
		set(CMAKE_INSTALL_PREFIX "C:/Program Files/nfx/datatypes" CACHE PATH "Installation directory" FORCE)
	else()
		set(CMAKE_INSTALL_PREFIX "/usr/local/nfx/datatypes" CACHE PATH "Installation directory" FORCE)
	endif()
	message(STATUS "Setting install prefix to '${CMAKE_INSTALL_PREFIX}'")
endif()

#----------------------------------------------
# Build type configuration
#----------------------------------------------

# Set default build type for single-config generators
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
	message(STATUS "Setting build type to '${CMAKE_BUILD_TYPE}' as none was specified")
endif()

# For multi-config generators (Visual Studio, Xcode), set available configurations
if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel" CACHE STRING "Available build configurations" FORCE)
	message(STATUS "Multi-config generator detected: ${CMAKE_CONFIGURATION_TYPES}")
else()
	message(STATUS "Single-config generator - Build type: ${CMAKE_BUILD_TYPE}")
endif()

#----------------------------------------------
# Version configuration
#----------------------------------------------

set(NFX_DATATYPES_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} CACHE INTERNAL "nfx-datatypes major version")
set(NFX_DATATYPES_VERSION_MINOR ${PROJECT_VERSION_MINOR} CACHE INTERNAL "nfx-datatypes minor version")
set(NFX_DATATYPES_VERSION_PATCH ${PROJECT_VERSION_PATCH} CACHE INTERNAL "nfx-datatypes patch version")
set(NFX_DATATYPES_VERSION       ${PROJECT_VERSION}       CACHE INTERNAL "nfx-datatypes version")

#----------------------------------------------
# Project metadata
#----------------------------------------------

set(NFX_DATATYPES_LICENSE "MIT")
set(NFX_DATATYPES_LICENSE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")

#----------------------------------------------
# Testing setup
#----------------------------------------------

enable_testing()

#----------------------------------------------
# Build options
#----------------------------------------------

# --- Build optimization ---
option(NFX_DATATYPES_USE_CACHE            "Enable compiler cache"              ON  )

# --- Library build types ---
option(NFX_DATATYPES_BUILD_STATIC         "Build static library"               ON  )
option(NFX_DATATYPES_BUILD_SHARED         "Build shared library"               OFF )

option(NFX_DATATYPES_BUILD_TESTS          "Build tests"                        OFF )
option(NFX_DATATYPES_BUILD_SAMPLES        "Build samples"                      OFF )
option(NFX_DATATYPES_BUILD_BENCHMARKS     "Build benchmarks"                   OFF )
option(NFX_DATATYPES_BUILD_DOCUMENTATION  "Build Doxygen documentation"        OFF )

# --- Installation ---
option(NFX_DATATYPES_INSTALL_PROJECT      "Install project"                    OFF )

# --- Packaging ---
option(NFX_DATATYPES_PACKAGE_SOURCE       "Enable source package generation"   OFF )
option(NFX_DATATYPES_PACKAGE_ARCHIVE      "Enable TGZ/ZIP package generation"  OFF )
option(NFX_DATATYPES_PACKAGE_DEB          "Enable DEB package generation"      OFF )
option(NFX_DATATYPES_PACKAGE_RPM          "Enable RPM package generation"      OFF )
option(NFX_DATATYPES_PACKAGE_WIX          "Enable WiX Windows installer (MSI)" OFF )

#----------------------------------------------
# CMake modules configuration
#----------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#----------------------------------------------
# Main configuration
#----------------------------------------------

include(nfxDataTypesBuildConfig)
include(nfxDataTypesDependencies)
include(nfxDataTypesSources)

#----------------------------------------------
# Installation and packaging
#----------------------------------------------

include(nfxDataTypesInstall)
include(nfxDataTypesPackaging)

#----------------------------------------------
# Targets
#----------------------------------------------

add_subdirectory(test)
add_subdirectory(samples)
add_subdirectory(benchmark)
add_subdirectory(doc)
