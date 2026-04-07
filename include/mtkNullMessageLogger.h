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
#ifndef mtkNullMessageLogger_H
#define mtkNullMessageLogger_H

#include "mtkLoggerNamespaceMacro.h"
#include "mtkLoggerLevel.h"
#include "mtkLoggerExportMacro.h"

#include <QString>
#include <QDateTime>
#include <QDebug>

MTK_LOGGER_BEGIN_NAMESPACE

/**
 * @brief Null object implementation of the log message stream.
 *
 * NullMessageLogger mirrors the full public interface of MessageLogger
 * so that logging macros can substitute it transparently when the requested
 * level does not pass the current filter. All operations are no-ops and
 * the destructor does not push anything to LoggerManager.
 *
 * Typical macro pattern:
 * @code
 * #define LOG_TRACE \
 *     (condition) ? MessageLogger(Level::Trace, ...).write() \
 *                 : NullMessageLogger().write()
 * @endcode
 */
class MTK_LOGGER_EXPORT NullMessageLogger
{
public:
    /**
     * @brief Mirrors the MessageLogger constructor signature.
     *        All arguments are accepted and silently discarded.
     */
    inline NullMessageLogger(Level            = Level::Trace,
                             const QString&   = QString(),
                             int              = 0,
                             const QString&   = QString(),
                             const QString&   = QString(),
                             const QString&   = QString(),
                             const QString&   = QString(),
                             const QDateTime& = QDateTime())
    {}

    /** @brief No-op destructor — nothing is pushed to any logger. */
    ~NullMessageLogger() = default;

    // Non-copyable
    NullMessageLogger(const NullMessageLogger&)            = delete;
    NullMessageLogger& operator=(const NullMessageLogger&) = delete;

    // ── No-op stream interfaces (mirroring MessageLogger::write) ─────────────

    /** @brief Accepts a message string and discards it. */
    inline void write(const QString&) {}

    /** @brief Accepts a printf-style format string and discards it. */
    inline void write(const char*, ...) {}

    /**
     * @brief Returns a QDebug stream bound to an internal sink.
     *        The sink is discarded on destruction without any forwarding.
     */
    inline QDebug write() { return QDebug(&m_sink); }

    // ── No-op accessors (mirroring MessageLogger accessors) ──────────────────

    inline Level     level()      const { return Level::Trace;  }
    inline QString   file()       const { return QString();     }
    inline int       line()       const { return 0;             }
    inline QString   function()   const { return QString();     }
    inline QString   category()   const { return QString();     }
    inline QString   message()    const { return QString();     }
    inline QString   threadId()   const { return QString();     }
    inline QString   modulename() const { return QString();     }
    inline QDateTime timestamp()  const { return QDateTime();   }

private:
    QString m_sink; ///< Throwaway buffer bound to QDebug; discarded on destruction
};

MTK_LOGGER_END_NAMESPACE

#endif // !mtkNullMessageLogger_H