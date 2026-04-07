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

#ifndef MTK_LOGGER_MACRO_H
#define MTK_LOGGER_MACRO_H

#include "mtkLoggerMessage.h"
#include "mtkAbstractLogger.h"

#include <QThread>

#ifndef US_MOUDLE_NAME
#define US_MOUDLE_NAME "mtkLogger"
#endif // !US_MOUDLE_NAME

#define MTK_LOGGER_CURRENT_THREADID QThread::currentThreadId()
#define MTK_LOGGER_CURRENT_THREADID_STR QString("0x%1").arg(          \
  reinterpret_cast<quintptr>(MTK_LOGGER_CURRENT_THREADID),            \
  sizeof(quintptr) * 2,                                               \
  16, QLatin1Char('0'))


#define MTK_INTERNAL_LOG(level, category)       \
    mtk::logger::MessageLogger(                 \
        mtk::logger::Level::level,              \
        __FILE__,                               \
        __LINE__,                               \
        Q_FUNC_INFO,                            \
        category,                               \
        MTK_LOGGER_CURRENT_THREADID_STR,        \
        US_MOUDLE_NAME)                         \


#define LOG_TRACE   MTK_INTERNAL_LOG(Trace, nullptr).write
#define LOG_DEBUG   MTK_INTERNAL_LOG(Debug, nullptr).write
#define LOG_INFO    MTK_INTERNAL_LOG(Info, nullptr).write
#define LOG_WARNING MTK_INTERNAL_LOG(Warning, nullptr).write
#define LOG_ERROR   MTK_INTERNAL_LOG(Error, nullptr).write
#define LOG_FATAL   MTK_INTERNAL_LOG(Fatal, nullptr).write

#define LOG_CTRACE(category)   MTK_INTERNAL_LOG(Trace, category).write()
#define LOG_CDEBUG(category)   MTK_INTERNAL_LOG(Debug, category).write()
#define LOG_CINFO(category)    MTK_INTERNAL_LOG(Info, category).write()
#define LOG_CWARNING(category) MTK_INTERNAL_LOG(Warning, category).write()
#define LOG_CERROR(category)   MTK_INTERNAL_LOG(Error, category).write()
#define LOG_CFATAL(category)   MTK_INTERNAL_LOG(Fatal, category).write()

#endif // !MTK_LOGGER_MACRO_H
