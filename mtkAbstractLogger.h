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
#ifndef mtkAbstractLogger_H
#define mtkAbstractLogger_H

#include "mtkLoggerNamespaceMacro.h"
#include "mtkLoggerLevel.h"
#include "mtkLoggerMessage.h"
#include "mtkAbstractAppender.h"
#include "mtkLoggerExportMacro.h"

#include <QString>
#include <QMap>
#include <QList>
#include <QSharedPointer>

MTK_LOGGER_BEGIN_NAMESPACE

/**
 * @brief Abstract named logger that filters messages by level and
 *        dispatches them to registered Appenders.
 *
 * A Logger is identified by a category name such as "" (global), "net",
 * or "net.tcp". Parent-child relationships are inferred from dot notation
 * and resolved entirely by LoggerManager — AbstractLogger has no
 * awareness of the logger hierarchy.
 *
 * Level filtering is applied here. Subclasses implement processMessage()
 * to define how and when messages are forwarded to Appenders.
 *
 * As the declared friend of AbstractAppender, AbstractLogger is the
 * only class permitted to call doProcessMessage() on an appender.
 */
class MTK_LOGGER_EXPORT AbstractLogger
{
public:
    explicit mtkAbstractLogger(const QString& category);
    virtual ~mtkAbstractLogger() = default;

    // Non-copyable
    mtkAbstractLogger(const mtkAbstractLogger&)            = delete;
    mtkAbstractLogger& operator=(const mtkAbstractLogger&) = delete;

    // ── Identity ─────────────────────────────────────────────────────────────

    /**
     * @brief Returns the category name of this logger (e.g. "net.tcp").
     */
    QString category() const;

    // ── Level control ─────────────────────────────────────────────────────────

    /**
     * @brief Sets the minimum level this logger will process.
     *        Messages below this level are silently dropped.
     */
    void  setLevel(Level level);
    Level level()             const;

    /**
     * @brief Returns true if the given level passes this logger's filter.
     */
    bool  isEnabled(Level level) const;

    // ── Appender management ───────────────────────────────────────────────────

    /**
     * @brief Adds an appender keyed by its name.
     *        If an appender with the same name already exists, it is replaced.
     */
    void addAppender(const QSharedPointer<mtkAbstractAppender>& appender);

    /**
     * @brief Removes the appender with the given name, if present.
     */
    void removeAppender(const QString& appenderName);

    /**
     * @brief Returns true if an appender with the given name is registered.
     */
    bool hasAppender(const QString& appenderName) const;

    /**
     * @brief Returns all appenders registered on this logger.
     */
    QList<QSharedPointer<mtkAbstractAppender>> appenders() const;

    // ── Logging ───────────────────────────────────────────────────────────────

    /**
     * @brief Applies the level filter and delegates to processMessage().
     *
     * Called by mtkLoggerManager for each Logger in the resolved hierarchy.
     * The Logger has no knowledge of its ancestors or siblings.
     *
     * @param msg The message to process.
     */
    void log(const MessageLogger& msg);

protected:
    /**
     * @brief Subclasses implement this to define the dispatch strategy.
     *
     * Called only when the message has already passed this logger's level
     * filter. Implementations typically iterate over m_appenders and call
     * doProcessMessage() on each one.
     *
     * @param msg The message to forward to appenders.
     */
    virtual void processMessage(const MessageLogger& msg) = 0;

private:
    QString                                            m_category;
    Level                                              m_level;
    QMap<QString, QSharedPointer<mtkAbstractAppender>> m_appenders;
};

MTK_LOGGER_END_NAMESPACE

#endif // !mtkAbstractLogger_H
