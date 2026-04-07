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
#include "Appender/mtkRollingFileAppender.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>

MTK_LOGGER_BEGIN_NAMESPACE

RollingFileAppender::RollingFileAppender(const QString& filePath,
                                               qint64         maxFileSize,
                                               int            maxBackupCount,
                                               const QString& name)
    : FileAppender(filePath, name)
    , m_basePath(filePath)
    , m_maxFileSize(maxFileSize)
    , m_maxBackupCount(maxBackupCount)
{
}

qint64 RollingFileAppender::maxFileSize() const  { return m_maxFileSize; }
void   RollingFileAppender::setMaxFileSize(qint64 bytes) { m_maxFileSize = bytes; }

int  RollingFileAppender::maxBackupCount() const { return m_maxBackupCount; }
void RollingFileAppender::setMaxBackupCount(int count)   { m_maxBackupCount = count; }

void RollingFileAppender::processMessage(const MessageLogger& msg)
{
    // Check whether the current file exceeds the size limit
    if (m_file && m_file->size() >= m_maxFileSize)
        rollOver();

    FileAppender::processMessage(msg);
}

void RollingFileAppender::rollOver()
{
    // Flush and close the current file
    if (m_stream) {
        m_stream->flush();
        m_file->close();
        delete m_stream;
        m_stream = nullptr;
        delete m_file;
        m_file = nullptr;
    }

    const QFileInfo info(m_basePath);
    const QString   dir      = info.absolutePath();
    const QString   baseName = info.completeBaseName(); // e.g. "app"
    const QString   suffix   = info.suffix();           // e.g. "log"

    // Build a helper lambda to get the backup file path for index i
    auto backupPath = [&](int i) -> QString {
        return QStringLiteral("%1/%2.%3.%4").arg(dir, baseName).arg(i).arg(suffix);
    };

    // Delete the oldest backup if it exists
    const QString oldest = backupPath(m_maxBackupCount);
    if (QFile::exists(oldest))
        QFile::remove(oldest);

    // Shift existing backups: app.4.log → app.5.log, ..., app.1.log → app.2.log
    for (int i = m_maxBackupCount - 1; i >= 1; --i) {
        const QString src  = backupPath(i);
        const QString dest = backupPath(i + 1);
        if (QFile::exists(src))
            QFile::rename(src, dest);
    }

    // Rename the current file to app.1.log
    QFile::rename(m_basePath, backupPath(1));

    // Open a new active file
    m_file = new QFile(m_basePath);
    if (m_file->open(QIODevice::Append | QIODevice::Text)) {
        m_stream = new QTextStream(m_file);
        m_stream->setCodec("UTF-8");
    }
}

MTK_LOGGER_END_NAMESPACE
