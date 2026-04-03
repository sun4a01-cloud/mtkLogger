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
#include "mtkAbstractAppender.h"

#include <QReadLocker>
#include <QWriteLocker>
#include <QFileInfo>

MTK_LOGGER_BEGIN_NAMESPACE

// ── Private implementation ───────────────────────────────────────────────────

struct mtkAbstractAppender::PrivateImp
{
    QString        name;
    QString        timestampFormat;
    Level          detailsLevel;
    QReadWriteLock lock;

    explicit PrivateImp(const QString& appenderName)
        : name(appenderName)
        , timestampFormat(QStringLiteral("yyyy-MM-dd HH:mm:ss.zzz"))
        , detailsLevel(Level::Trace)
    {}
};

// ── Construction ─────────────────────────────────────────────────────────────

mtkAbstractAppender::mtkAbstractAppender(const QString& name)
    : imp(new PrivateImp(name))
{}

mtkAbstractAppender::~mtkAbstractAppender()
{}

// ── Identity ─────────────────────────────────────────────────────────────────

QString mtkAbstractAppender::name() const
{
    QReadLocker locker(&imp->lock);
    return imp->name;
}

// ── Timestamp format ─────────────────────────────────────────────────────────

void mtkAbstractAppender::setTimestampFormat(const QString& format)
{
    QWriteLocker locker(&imp->lock);
    imp->timestampFormat = format;
}

QString mtkAbstractAppender::timestampFormat() const
{
    QReadLocker locker(&imp->lock);
    return imp->timestampFormat;
}

// ── Level filtering ──────────────────────────────────────────────────────────

Level mtkAbstractAppender::detailsLevel() const
{
    QReadLocker locker(&imp->lock);
    return imp->detailsLevel;
}

void mtkAbstractAppender::setDetailsLevel(Level level)
{
    QWriteLocker locker(&imp->lock);
    imp->detailsLevel = level;
}

void mtkAbstractAppender::setDetailsLevel(const QString& levelStr)
{
    bool ok = false;
    Level level = levelFromString(levelStr, &ok);
    if (ok)
        setDetailsLevel(level);
}

// ── Protected interface (accessible only by AbstractLogger) ──────────────────

QReadWriteLock& mtkAbstractAppender::readWriteLock() const
{
    return imp->lock;
}

void mtkAbstractAppender::doProcessMessage(const MessageLogger& msg)
{
    // Apply the appender's own level filter
    {
        QReadLocker locker(&imp->lock);
        if (msg.m_level < imp->detailsLevel)
            return;
    }

    processMessage(msg);
}

// ── Default formatter ────────────────────────────────────────────────────────

QString mtkAbstractAppender::format(const MessageLogger& msg) const
{
    // [2024-01-01 12:00:00.000] [INFO   ] [module] [category] (file:line func) message
    QReadLocker locker(&imp->lock);

    const QString timestamp = msg.m_timestamp.toString(imp->timestampFormat);
    const QString level     = levelToString(msg.m_level).leftJustified(7);
    const QString fileName  = QFileInfo(msg.m_file).fileName();

    return QString("[%1] [%2] [%3] [%4] (%5:%6 %7) %8")
           .arg(timestamp)
           .arg(level)
           .arg(msg.m_modulename)
           .arg(msg.m_category)
           .arg(fileName)
           .arg(msg.m_line)
           .arg(msg.m_function)
           .arg(msg.m_message);
}

MTK_LOGGER_END_NAMESPACE
