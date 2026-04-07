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
#ifndef mtkRollingFileAppender_H
#define mtkRollingFileAppender_H

#include "mtkFileAppender.h"

MTK_LOGGER_BEGIN_NAMESPACE

/**
 * @brief File appender with size-based rolling policy.
 *
 * When the current log file exceeds maxFileSize(), it is renamed to
 * "<baseName>.<index>.log" and a new file is opened. At most
 * maxBackupCount() backup files are kept; older ones are deleted.
 *
 * Example with basePath="app.log", maxBackupCount=3:
 *   app.log          ← current
 *   app.1.log        ← most recent backup
 *   app.2.log
 *   app.3.log        ← oldest backup (deleted on next roll)
 */
class RollingFileAppender : public FileAppender
{
public:
    /**
     * @brief Constructs a RollingFileAppender.
     * @param filePath       Path to the active log file.
     * @param maxFileSize    Maximum file size in bytes before rolling (default: 10 MB).
     * @param maxBackupCount Number of backup files to retain (default: 5).
     * @param name           Appender identifier (default: "rollingFile").
     */
    explicit RollingFileAppender(
        const QString& filePath,
        qint64         maxFileSize    = 10 * 1024 * 1024,
        int            maxBackupCount = 5,
        const QString& name          = QStringLiteral("rollingFile"));

    /**
     * @brief Returns the maximum file size in bytes before a roll occurs.
     */
    qint64 maxFileSize() const;
    void   setMaxFileSize(qint64 bytes);

    /**
     * @brief Returns the maximum number of backup files to retain.
     */
    int  maxBackupCount() const;
    void setMaxBackupCount(int count);

protected:
  virtual void processMessage(const MessageLogger& msg) override;

private:
    /**
     * @brief Performs the roll: renames existing backups and opens a new file.
     */
    void rollOver();

    QString m_basePath;
    qint64  m_maxFileSize;
    int     m_maxBackupCount;
};

MTK_LOGGER_END_NAMESPACE

#endif // !mtkRollingFileAppender_H
