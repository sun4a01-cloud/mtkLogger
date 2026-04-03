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
#include "mtkLoggerManager.h"
#include <QMutexLocker>

MTK_LOGGER_BEGIN_NAMESPACE

// ── Static members ────────────────────────────────────────────────────────────

mtkLoggerManager* mtkLoggerManager::m_instance      = nullptr;
QMutex            mtkLoggerManager::m_instanceMutex;

// ── Singleton ─────────────────────────────────────────────────────────────────

mtkLoggerManager* mtkLoggerManager::instance()
{
    QMutexLocker locker(&m_instanceMutex);
    if (!m_instance)
        m_instance = new mtkLoggerManager();
    return m_instance;
}

// ── Logger registry ───────────────────────────────────────────────────────────

mtkAbstractLogger* mtkLoggerManager::getLogger(const QString& category)
{
    QMutexLocker locker(&m_mutex);

    auto it = m_loggers.find(category);
    if (it != m_loggers.end())
        return it->data();

    QSharedPointer<mtkAbstractLogger> logger(new mtkAbstractLogger(category));
    m_loggers.insert(category, logger);
    return logger.data();
}

bool mtkLoggerManager::hasLogger(const QString& category) const
{
    QMutexLocker locker(&m_mutex);
    return m_loggers.contains(category);
}

void mtkLoggerManager::removeLogger(const QString& category)
{
    QMutexLocker locker(&m_mutex);
    m_loggers.remove(category);
}

// ── Dispatching ───────────────────────────────────────────────────────────────

void mtkLoggerManager::log(const MessageLogger& msg, const QString& category)
{
    // Dispatch to the target logger
    getLogger(category)->log(msg);

    // Walk up the ancestor chain and dispatch to each ancestor logger
    QString current = parentCategory(category);
    while (!current.isNull()) {
        if (hasLogger(current))
            getLogger(current)->log(msg);
        current = parentCategory(current);
    }
}

// ── Private helpers ───────────────────────────────────────────────────────────

QString mtkLoggerManager::parentCategory(const QString& category)
{
    const int lastDot = category.lastIndexOf(QLatin1Char('.'));
    if (lastDot <= 0)
        return QString();
    return category.left(lastDot);
}


MTK_LOGGER_END_NAMESPACE
