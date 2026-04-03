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
#ifndef mtkAbstractAppender_H
#define mtkAbstractAppender_H

#include "mtkLoggerNamespaceMacro.h"
#include "mtkLoggerLevel.h"
#include "mtkLoggerMessage.h"

#include <QString>
#include <QReadWriteLock>
#include <QSharedPointer>

MTK_LOGGER_BEGIN_NAMESPACE

/**
 * @brief Abstract base class for all log appenders.
 *
 * An Appender is responsible for formatting a MessageLogger and writing
 * it to a specific output target (console, file, etc.).
 *
 * Access control:
 *   - doProcessMessage() and processMessage() are protected.
 *   - Only AbstractLogger (declared as friend) may invoke them,
 *     keeping the processing interface hidden from all other callers.
 *
 * Subclasses must implement processMessage() to perform actual output.
 */
class mtkAbstractAppender
{
public:
    explicit mtkAbstractAppender(const QString& name);
    virtual ~mtkAbstractAppender();

    // Non-copyable
    mtkAbstractAppender(const mtkAbstractAppender&)            = delete;
    mtkAbstractAppender& operator=(const mtkAbstractAppender&) = delete;

    // ── Identity ────────────────────────────────────────────────────────────

    /**
     * @brief Returns the name of this appender.
     */
    QString name() const;

    // ── Timestamp formatting ─────────────────────────────────────────────────

    /**
     * @brief Sets the timestamp format string used when formatting messages.
     *        Defaults to "yyyy-MM-dd HH:mm:ss.zzz".
     */
    void    setTimestampFormat(const QString& format);
    QString timestampFormat() const;

    // ── Level filtering ──────────────────────────────────────────────────────

    /**
     * @brief Returns the minimum level this appender will process.
     *
     * This threshold is independent of the Logger's own level filter.
     * Any MessageLogger whose level is below detailsLevel() is silently
     * dropped by doProcessMessage() before reaching processMessage().
     */
    Level detailsLevel() const;

    /**
     * @brief Sets the minimum level this appender will process.
     * @param level The new threshold level.
     */
    void setDetailsLevel(Level level);

    /**
     * @brief Sets the minimum level from a string (case-insensitive).
     *        Unrecognized strings are ignored and the level remains unchanged.
     * @param levelStr e.g. "debug", "Warning", "ERROR"
     */
    void setDetailsLevel(const QString& levelStr);

protected:
    // ── Interface reserved for AbstractLogger ────────────────────────────────

    /**
     * @brief AbstractLogger is the only class permitted to call
     *        doProcessMessage() and processMessage().
     */
    friend class AbstractLogger;

    /**
     * @brief Returns a reference to the appender's read-write lock.
     *
     * AbstractLogger may acquire this lock before dispatching messages
     * to coordinate concurrent access across multiple appenders.
     */
    QReadWriteLock& readWriteLock() const;

    /**
     * @brief Entry point called by AbstractLogger to dispatch a log message.
     *
     * Applies the detailsLevel() filter, then delegates to processMessage().
     *
     * @param msg The log message to process.
     */
    void doProcessMessage(const MessageLogger& msg);

    /**
     * @brief Subclasses implement this to perform formatting and output.
     *
     * Called only when the message has already passed both the Logger's
     * level filter and this appender's detailsLevel() filter.
     *
     * @param msg The log message to format and write.
     */
    virtual void processMessage(const MessageLogger& msg) = 0;

    /**
     * @brief Formats a MessageLogger into a single output line.
     *
     * Default format:
     *   [timestamp] [LEVEL  ] [module] [category] (file:line function) message
     *
     * Subclasses may override this to customise formatting.
     */
    virtual QString format(const MessageLogger& msg) const;

private:
    struct PrivateImp;
    QSharedPointer<PrivateImp> imp;
};

MTK_LOGGER_END_NAMESPACE

#endif // !mtkAbstractAppender_H
