// Philipp Neufeld, 2023

#ifndef QPT_PLATFORM_H_
#define QPT_PLATFORM_H_

// Includes
#include <cstdint>

// compiler
#if defined(_MSC_VER) && !defined(__clang__)
#ifndef QPT_COMPILER_MSVC
#define QPT_COMPILER_MSVC
#endif  // !QPT_COMPILER_MSVC
#elif defined(__clang__)
#ifndef QPT_COMPILER_CLANG
#define QPT_COMPILER_CLANG
#endif  // !QPT_COMPILER_CLANG
#elif defined(__GNUC__)
#ifndef QPT_COMPILER_GNUC
#define QPT_COMPILER_GNUC
#endif  // !QPT_COMPILER_GNUC
#else
#ifndef QPT_COMPILER_UNKNOWN
#define QPT_COMPILER_UNKNOWN
#endif  // !QPT_COMPILER_UNKNOWN
#endif

// OS macro
#if defined(_WIN32)
#ifndef QPT_PLATFORM_WINDOWS
#define QPT_PLATFORM_WINDOWS
#endif  // !QPT_PLATFORM_WINDOWS
#elif defined(__linux__)
#ifndef QPT_PLATFORM_LINUX
#define QPT_PLATFORM_LINUX
#endif  // !QPT_PLATFORM_LINUX
#elif defined(__APPLE__)
#ifndef QPT_PLATFORM_MACOS
#define QPT_PLATFORM_MACOS
#endif  // !QPT_PLATFORM_MACOS
#else
#ifndef QPT_PLATFORM_UNKNOWN
#define QPT_PLATFORM_UNKNOWN
#endif  // !QPT_PLATFORM_UNKNOWN
#endif

// architecture
// https://stackoverflow.com/questions/152016/detecting-cpu-architecture-compile-time
#if defined(__x86_64__) || defined(_M_X64)
#define QPT_ARCH_X86_64
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define QPT_ARCH_X86_32
#elif defined(__aarch64__) || defined(_M_ARM64)
#define QPT_ARCH_ARM64
#else
#define QPT_ARCH_UNKNOWN
#endif

// c++ version
#if defined QPT_COMPILER_MSVC
#define QPT_CPLUSPLUS _MSVC_LANG
#else
#define QPT_CPLUSPLUS __cplusplus
#endif

#if QPT_CPLUSPLUS >= 201103L
#define QPT_HAS_CXX11
#endif

#if QPT_CPLUSPLUS >= 201402L
#define QPT_HAS_CXX14
#endif

#if QPT_CPLUSPLUS >= 201703L
#define QPT_HAS_CXX17
#endif

#if QPT_CPLUSPLUS >= 202002L
#define QPT_HAS_CXX20
#endif

// debug macros
#if defined(DEBUG) || defined(_DEBUG)
#ifndef QPT_DEBUG
#define QPT_DEBUG
#endif
#endif

// define the library namespace
namespace QPT {}

#endif
