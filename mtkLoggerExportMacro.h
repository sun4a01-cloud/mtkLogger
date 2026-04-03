/*============================================================================
  Library: mtkLogger
  Copyright (c) Your Organization Name
  All rights reserved.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
============================================================================*/
#ifndef mtkLoggerExportMacro_H
#define mtkLoggerExportMacro_H

/**
 * @file mtkLoggerExportMacro.h
 * @brief Cross-platform DLL export/import macro definitions.
 *
 * This header defines MTK_LOGGER_EXPORT for marking classes and functions
 * that should be exported from the mtkLogger shared library.
 *
 * Usage:
 *   class MTK_LOGGER_EXPORT MyClass { ... };
 *   MTK_LOGGER_EXPORT void myFunction();
 *
 * Build configuration:
 *   - When building mtkLogger as a shared library, define MTK_LOGGER_BUILDING_DLL
 *   - When building mtkLogger as a static library, define MTK_LOGGER_STATIC_DEFINE
 *   - When using mtkLogger, no defines are needed (symbols will be imported)
 */

// Determine if we're building a static or shared library
#if defined(MTK_LOGGER_STATIC_DEFINE)
    // Static library - no export needed
    #define MTK_LOGGER_EXPORT
    #define MTK_LOGGER_NO_EXPORT

#elif defined(_WIN32) || defined(_WIN64)
    // Windows DLL
    #if defined(MTK_LOGGER_BUILDING_DLL)
        // Building the DLL - export symbols
        #define MTK_LOGGER_EXPORT __declspec(dllexport)
    #else
        // Using the DLL - import symbols
        #define MTK_LOGGER_EXPORT __declspec(dllimport)
    #endif
    #define MTK_LOGGER_NO_EXPORT

#else
    // GCC/Clang on Linux, macOS, etc.
    #if defined(__GNUC__) || defined(__clang__)
        #define MTK_LOGGER_EXPORT __attribute__((visibility("default")))
        #define MTK_LOGGER_NO_EXPORT __attribute__((visibility("hidden")))
    #else
        #define MTK_LOGGER_EXPORT
        #define MTK_LOGGER_NO_EXPORT
    #endif
#endif

#endif // !mtkLoggerExportMacro_H
