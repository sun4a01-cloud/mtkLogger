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
#ifndef mtkLoggerLevel_H
#define mtkLoggerLevel_H

#include "mtkLoggerNamespaceMacro.h"
#include <QString>

MTK_LOGGER_BEGIN_NAMESPACE

/**
 * @brief Log severity levels, ordered from lowest to highest.
 */
enum class Level
{
    Trace   = 0,  ///< Fine-grained diagnostic information
    Debug   = 1,  ///< Debugging information
    Info    = 2,  ///< General informational messages
    Warning = 3,  ///< Potentially harmful situations
    Error   = 4,  ///< Error events that might still allow the app to continue
    Fatal   = 5   ///< Severe errors that will presumably abort the application
};

/**
 * @brief Converts a Level value to its string representation.
 * @param level The log level.
 * @return A human-readable string such as "TRACE", "DEBUG", etc.
 */
inline QString levelToString(Level level)
{
    switch (level)
    {
    case Level::Trace:   return QStringLiteral("TRACE");
    case Level::Debug:   return QStringLiteral("DEBUG");
    case Level::Info:    return QStringLiteral("INFO");
    case Level::Warning: return QStringLiteral("WARNING");
    case Level::Error:   return QStringLiteral("ERROR");
    case Level::Fatal:   return QStringLiteral("FATAL");
    default:             return QStringLiteral("UNKNOWN");
    }
}

/**
 * @brief Converts a string to the corresponding Level value.
 * @param str A case-insensitive level string.
 * @param ok  Optional output parameter; set to false if conversion fails.
 * @return The matching Level, or Level::Info if the string is unrecognized.
 */
inline Level levelFromString(const QString& str, bool* ok = nullptr)
{
    const QString upper = str.toUpper();
    if (ok) *ok = true;

    if (upper == QLatin1String("TRACE"))   return Level::Trace;
    if (upper == QLatin1String("DEBUG"))   return Level::Debug;
    if (upper == QLatin1String("INFO"))    return Level::Info;
    if (upper == QLatin1String("WARNING")) return Level::Warning;
    if (upper == QLatin1String("ERROR"))   return Level::Error;
    if (upper == QLatin1String("FATAL"))   return Level::Fatal;

    if (ok) *ok = false;
    return Level::Info;
}

MTK_LOGGER_END_NAMESPACE

#endif // !mtkLoggerLevel_H
