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
#include "mtkFileAppender.h"
#include <QDebug>

MTK_LOGGER_BEGIN_NAMESPACE

mtkFileAppender::mtkFileAppender(const QString& filePath, const QString& name)
    : mtkAppender(name)
    , m_file(nullptr)
    , m_stream(nullptr)
{
    openFile(filePath);
}

mtkFileAppender::~mtkFileAppender()
{
    if (m_file && m_file->isOpen()) {
        m_stream->flush();
        m_file->close();
    }
    delete m_stream;
    delete m_file;
}

bool mtkFileAppender::isOpen() const
{
    return m_file && m_file->isOpen();
}

QString mtkFileAppender::filePath() const
{
    return m_file ? m_file->fileName() : QString();
}

bool mtkFileAppender::openFile(const QString& filePath)
{
    m_file = new QFile(filePath);
    if (!m_file->open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "[mtkFileAppender] Failed to open log file:" << filePath;
        delete m_file;
        m_file = nullptr;
        return false;
    }
    m_stream = new QTextStream(m_file);
    m_stream->setCodec("UTF-8");
    return true;
}

void mtkFileAppender::append(const Msg& msg)
{
    if (!m_stream)
        return;

    *m_stream << format(msg) << '\n';
    m_stream->flush();
}

MTK_LOGGER_END_NAMESPACE
