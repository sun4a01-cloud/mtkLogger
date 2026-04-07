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
#include "mtkLoggerMessage.h"

#include <QString>
#include "mtkLoggerManager.h"

#include <cstdarg>

MTK_LOGGER_BEGIN_NAMESPACE

// ── Destructor: push stream to LoggerManager ─────────────────────────────────

MessageLogger::~MessageLogger()
{
    // Only dispatch if there is something to log
    if (m_message.isEmpty())
        return;

    LoggerManager::instance()->log(*this, m_category);
}

// ── write() overloads ────────────────────────────────────────────────────────

void MessageLogger::write(const QString& msg)
{
    m_message = msg;
}

void MessageLogger::write(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    m_message = QString::vasprintf(fmt, args);
    va_end(args);
}

QDebug MessageLogger::write()
{
    // QDebug bound to m_message: every << operator appends directly into it.
    // The QDebug object is destroyed at the end of the calling expression,
    // flushing its internal buffer into m_message before ~MessageLogger runs.
    return QDebug(&m_message);
}

MTK_LOGGER_END_NAMESPACE
