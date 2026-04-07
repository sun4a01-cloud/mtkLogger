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
#ifndef mtkConsoleAppender_H
#define mtkConsoleAppender_H

#include "mtkAbstractAppender.h"

MTK_LOGGER_BEGIN_NAMESPACE

/**
 * @brief Appender that writes formatted log messages to the console.
 *
 * Output is routed through Qt's message handler:
 *   Trace / Debug            → qDebug()
 *   Info                     → qInfo()
 *   Warning                  → qWarning()
 *   Error                    → qCritical()
 *   Fatal                    → qFatal()
 */
class ConsoleAppender : public AbstractAppender
{
public:
    /**
     * @brief Constructs a ConsoleAppender with the given name.
     * @param name Appender identifier (default: "console").
     */
    explicit ConsoleAppender(const QString& name = QStringLiteral("console"));

protected:
  virtual void processMessage(const MessageLogger& msg) override;
};

MTK_LOGGER_END_NAMESPACE

#endif // !mtkConsoleAppender_H
