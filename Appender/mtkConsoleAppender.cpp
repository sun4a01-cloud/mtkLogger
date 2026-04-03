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
#include "mtkConsoleAppender.h"
#include <QDebug>

MTK_LOGGER_BEGIN_NAMESPACE

mtkConsoleAppender::mtkConsoleAppender(const QString& name)
    : mtkAppender(name)
{
}

void mtkConsoleAppender::append(const Msg& msg)
{
    const QString line = format(msg);

    switch (msg.m_level)
    {
    case Level::Trace:
    case Level::Debug:
        qDebug().noquote()    << line; break;
    case Level::Info:
        qInfo().noquote()     << line; break;
    case Level::Warning:
        qWarning().noquote()  << line; break;
    case Level::Error:
        qCritical().noquote() << line; break;
    case Level::Fatal:
        qFatal("%s", qPrintable(line)); break;
    }
}

MTK_LOGGER_END_NAMESPACE
