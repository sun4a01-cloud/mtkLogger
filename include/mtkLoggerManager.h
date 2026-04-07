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
#ifndef LoggerManager_H
#define LoggerManager_H

#include "mtkLoggerNamespaceMacro.h"
#include "mtkAbstractLogger.h"
#include "mtkLoggerMessage.h"
#include "mtkLoggerExportMacro.h"

#include <QString>
#include <QMap>
#include <QList>
#include <QSharedPointer>
#include <QMutex>

MTK_LOGGER_BEGIN_NAMESPACE

/**
 * @brief Central registry that creates, owns, and looks up named loggers.
 *
 * Usage:
 * @code
 *   auto* mgr = LoggerManager::instance();
 *   AbstractLogger* net = mgr->getLogger("net");
 *   net->addAppender(...);
 *   mgr->log(msg, "net.tcp");   // dispatches through "net.tcp" -> "net"
 * @endcode
 *
 * Parent resolution:
 *   "net.tcp"  -> ancestors: ["net"]
 *   "net"      -> ancestors: []  (does NOT default to "")
 *   ""         -> ancestors: []
 */
class MTK_LOGGER_EXPORT LoggerManager
{
public:
    /** @brief Returns the singleton instance. */
    static LoggerManager* instance();

    // Non-copyable
    LoggerManager(const LoggerManager&)            = delete;
    LoggerManager& operator=(const LoggerManager&) = delete;

    /**
     * @brief Returns the logger for the given category, creating it if needed.
     * @param category Logger name, e.g. "", "net", "net.tcp".
     */
    AbstractLogger* getLogger(const QString& category);

    /**
     * @brief Returns true if a logger with the given category already exists.
     */
    bool hasLogger(const QString& category) const;

    /**
     * @brief Removes a logger from the registry.
     *        Does nothing if the category is not found.
     */
    void removeLogger(const QString& category);

    /**
     * @brief Dispatches a MessageLogger through the logger hierarchy.
     *
     * Resolves all ancestor loggers via dot-notation, collects their
     * appenders, then calls AbstractLogger::log() with the inherited set.
     *
     * @param msg      The log message.
     * @param category The originating logger category.
     */
    void log(const MessageLogger& msg, const QString& category);

private:
    LoggerManager() = default;

    /**
     * @brief Returns the direct parent category string.
     *        Returns a null QString if there is no parent.
     *
     * Example: parentCategory("net.tcp") == "net"
     *          parentCategory("net")     == QString()
     */
    static QString parentCategory(const QString& category);

    static LoggerManager* m_instance;
    static QMutex            m_instanceMutex;

    mutable QMutex                                       m_mutex;
    QMap<QString, QSharedPointer<AbstractLogger>>     m_loggers;
};

MTK_LOGGER_END_NAMESPACE

#endif // !LoggerManager_H