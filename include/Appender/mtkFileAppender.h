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
#ifndef mtkFileAppender_H
#define mtkFileAppender_H

#include <mtkAbstractAppender.h>

#include <QFile>
#include <QTextStream>

MTK_LOGGER_BEGIN_NAMESPACE

/**
 * @brief Appender that writes formatted log messages to a plain file.
 *
 * The file is opened in append mode and kept open for the lifetime
 * of the appender. The stream is flushed after every write.
 */
class FileAppender : public AbstractAppender
{
public:
    /**
     * @brief Constructs a FileAppender.
     * @param filePath Absolute or relative path to the log file.
     * @param name     Appender identifier (default: "file").
     */
    explicit FileAppender(const QString& filePath,
                             const QString& name = QStringLiteral("file"));
    ~FileAppender() override;

    /**
     * @brief Returns true if the log file was successfully opened.
     */
    bool isOpen() const;

    /**
     * @brief Returns the path of the log file.
     */
    QString filePath() const;

protected:
  virtual void processMessage(const MessageLogger& msg) override;

    QFile*       m_file;
    QTextStream* m_stream;

private:
    bool openFile(const QString& filePath);
};

MTK_LOGGER_END_NAMESPACE

#endif // !mtkFileAppender_H
