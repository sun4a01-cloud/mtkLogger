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
#ifndef MTKLOGGER_H
#define MTKLOGGER_H

#include "mtkAbstractLogger.h"
#include "mtkLoggerExportMacro.h"

MTK_LOGGER_BEGIN_NAMESPACE

/**
 * @brief Concrete logger that synchronously dispatches messages to all
 *        registered Appenders.
 *
 * This is the default Logger implementation created by LoggerManager.
 * It iterates over all attached Appenders and calls doProcessMessage()
 * on each one.
 */
class MTK_LOGGER_EXPORT Logger : public AbstractLogger
{
public:
    explicit Logger(const QString& category);
    ~Logger() override;

protected:
    void processMessage(const MessageLogger& msg) override;
};

MTK_LOGGER_END_NAMESPACE

#endif // !MTKLOGGER_H
