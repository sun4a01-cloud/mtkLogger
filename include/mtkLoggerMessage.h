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
#ifndef mtkLoggerMessage_H
#define mtkLoggerMessage_H

#include "mtkLoggerNamespaceMacro.h"
#include "mtkLoggerLevel.h"
#include "mtkLoggerExportMacro.h"

#include <QString>
#include <QDateTime>
#include <QDebug>

MTK_LOGGER_BEGIN_NAMESPACE

/**
 * @brief Log message class and stream entry point.
 *
 * MessageLogger carries all metadata captured at the call site.
 * It provides three write() overloads for collecting the message text:
 *
 *   - write(const QString&)   — assign the message directly
 *   - write(const char*, ...) — printf-style formatting
 *   - write()                 — returns a QDebug stream bound to m_message,
 *                               enabling the familiar "<<" syntax:
 *                               @code
 *                               LOG_TRACE << "connected" << ip << port;
 *                               @endcode
 *
 * On destruction, the accumulated message is automatically pushed to
 * LoggerManager, making the temporary object pattern safe and concise.
 */
class MTK_LOGGER_EXPORT MessageLogger
{
public:
    /**
     * @brief Constructs a MessageLogger with all metadata populated.
     *        The message text is filled later via write().
     */
    MessageLogger(Level            level,
                  const QString&   file,
                  int              line,
                  const QString&   function,
                  const QString&   category,
                  const QString&   threadId,
                  const QString&   modulename,
                  const QDateTime& timestamp = QDateTime::currentDateTime())
        : m_level     (level)
        , m_file      (file)
        , m_line      (line)
        , m_function  (function)
        , m_category  (category)
        , m_threadId  (threadId)
        , m_modulename(modulename)
        , m_timestamp (timestamp)
    {}

    /**
     * @brief Destructor — pushes the completed message to LoggerManager.
     *
     * This is the automatic push-stream trigger. When the temporary
     * MessageLogger goes out of scope (end of log statement), the
     * accumulated m_message is dispatched to the logger hierarchy.
     */
    ~MessageLogger();

    // Non-copyable — each MessageLogger represents one unique log event
    MessageLogger(const MessageLogger&)            = delete;
    MessageLogger& operator=(const MessageLogger&) = delete;

    // ── Stream interfaces ────────────────────────────────────────────────────

    /**
     * @brief Assigns the message text directly.
     * @param msg The message string.
     */
    void write(const QString& msg);

    /**
     * @brief Assigns the message text using printf-style formatting.
     * @param fmt Format string, followed by variadic arguments.
     */
    void write(const char* fmt, ...);

    /**
     * @brief Returns a QDebug stream bound to m_message.
     *
     * The returned QDebug writes directly into m_message via
     * QDebug(&m_message), enabling the "<<" operator chain.
     * The stream is flushed when the QDebug object is destroyed
     * at the end of the log statement — before ~MessageLogger runs.
     *
     * @code
     * MessageLogger(Level::Debug, ...).write() << "value =" << 42;
     * @endcode
     */
    QDebug write();

    // ── Accessors ────────────────────────────────────────────────────────────

    Level     level()      const { return m_level;      }
    QString   file()       const { return m_file;       }
    int       line()       const { return m_line;       }
    QString   function()   const { return m_function;   }
    QString   category()   const { return m_category;   }
    QString   message()    const { return m_message;    }
    QString   threadId()   const { return m_threadId;   }
    QString   modulename() const { return m_modulename; }
    QDateTime timestamp()  const { return m_timestamp;  }

private:
    Level     m_level;
    QString   m_file;
    int       m_line;
    QString   m_function;
    QString   m_category;
    QString   m_message;      ///< Filled by write(); QDebug streams into this
    QString   m_threadId;
    QString   m_modulename;
    QDateTime m_timestamp;
};

MTK_LOGGER_END_NAMESPACE

#endif // !mtkLoggerMessage_H