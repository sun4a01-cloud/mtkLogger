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
#include "mtkAbstractLogger.h"

MTK_LOGGER_BEGIN_NAMESPACE

mtkAbstractLogger::mtkAbstractLogger(const QString& category)
    : m_category(category)
    , m_level(Level::Debug)
{}

// ── Identity ─────────────────────────────────────────────────────────────────

QString mtkAbstractLogger::category() const { return m_category; }

// ── Level control ─────────────────────────────────────────────────────────────

Level mtkAbstractLogger::level()             const { return m_level;        }
void  mtkAbstractLogger::setLevel(Level level)     { m_level = level;       }
bool  mtkAbstractLogger::isEnabled(Level level) const { return level >= m_level; }

// ── Appender management ───────────────────────────────────────────────────────

void mtkAbstractLogger::addAppender(const QSharedPointer<mtkAbstractAppender>& appender)
{
    if (!appender)
        return;
    m_appenders.insert(appender->name(), appender);
}

void mtkAbstractLogger::removeAppender(const QString& appenderName)
{
    m_appenders.remove(appenderName);
}

bool mtkAbstractLogger::hasAppender(const QString& appenderName) const
{
    return m_appenders.contains(appenderName);
}

QList<QSharedPointer<mtkAbstractAppender>> mtkAbstractLogger::appenders() const
{
    return m_appenders.values();
}

// ── Logging ───────────────────────────────────────────────────────────────────

void mtkAbstractLogger::log(const MessageLogger& msg)
{
    // Level filter — drop messages below this logger's threshold
    if (!isEnabled(msg.level()))
        return;

    // Delegate dispatch strategy to the subclass
    processMessage(msg);
}

MTK_LOGGER_END_NAMESPACE
