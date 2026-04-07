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

#include <QtCore/QtGlobal>

/**
 * @brief DLL export / import macro for the mtkLogger library.
 *
 * When building mtkLogger as a shared library, define MTK_LOGGER_EXPORTS
 * in the project (.pro or CMakeLists.txt) to export symbols:
 *
 *   DEFINES += MTK_LOGGER_EXPORTS
 *
 * Consumers of the shared library do not need to define anything ¡ª
 * MTK_LOGGER_EXPORT will automatically resolve to Q_DECL_IMPORT.
 *
 * When building as a static library or including sources directly,
 * MTK_LOGGER_EXPORT expands to nothing.
 */
#if defined(MTK_LOGGER_STATIC)
#  define MTK_LOGGER_EXPORT
#elif defined(MTK_LOGGER_EXPORTS)
#  define MTK_LOGGER_EXPORT Q_DECL_EXPORT
#else
#  define MTK_LOGGER_EXPORT Q_DECL_IMPORT
#endif

#endif // !mtkLoggerExportMacro_H
