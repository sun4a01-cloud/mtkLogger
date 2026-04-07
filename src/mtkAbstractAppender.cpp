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

#include <QMutexLocker>

#include <QReadLocker>
#include <QWriteLocker>
#include <QFileInfo>

MTK_LOGGER_BEGIN_NAMESPACE

// ── Private implementation ───────────────────────────────────────────────────

struct AbstractAppender::PrivateImp
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

AbstractAppender::AbstractAppender(const QString& name)
    : imp(new PrivateImp(name))
{}

AbstractAppender::~AbstractAppender()
{}

// ── Identity ─────────────────────────────────────────────────────────────────

QString AbstractAppender::name() const
{
    QReadLocker locker(&imp->lock);
    return imp->name;
}

// ── Timestamp format ─────────────────────────────────────────────────────────

void AbstractAppender::setTimestampFormat(const QString& format)
{
    QWriteLocker locker(&imp->lock);
    imp->timestampFormat = format;
}

QString AbstractAppender::timestampFormat() const
{
    QReadLocker locker(&imp->lock);
    return imp->timestampFormat;
}

// ── Level filtering ──────────────────────────────────────────────────────────

Level AbstractAppender::detailsLevel() const
{
    QReadLocker locker(&imp->lock);
    return imp->detailsLevel;
}

void AbstractAppender::setDetailsLevel(Level level)
{
    QWriteLocker locker(&imp->lock);
    imp->detailsLevel = level;
}

void AbstractAppender::setDetailsLevel(const QString& levelStr)
{
    bool ok = false;
    Level level = levelFromString(levelStr, &ok);
    if (ok)
        setDetailsLevel(level);
}

// ── Protected interface (accessible only by AbstractLogger) ──────────────────

QReadWriteLock& AbstractAppender::readWriteLock() const
{
    return imp->lock;
}

void AbstractAppender::doProcessMessage(const MessageLogger& msg)
{
    // Apply the appender's own level filter
    {
        QReadLocker locker(&imp->lock);
        if (msg.level() < imp->detailsLevel)
            return;
    }

    processMessage(msg);
}

// ── Default formatter ────────────────────────────────────────────────────────

QString AbstractAppender::format(const MessageLogger& msg) const
{
    // [2024-01-01 12:00:00.000] [INFO   ] [module] [category] (file:line func) message
    QReadLocker locker(&imp->lock);

    const QString timestamp = msg.timestamp().toString(imp->timestampFormat);
    const QString level     = levelToString(msg.level()).leftJustified(7);
    const QString fileName  = QFileInfo(msg.file()).fileName();

    return QString("[%1] [%2] [%3] [%4] (%5:%6 %7) %8")
           .arg(timestamp)
           .arg(level)
           .arg(msg.modulename())
           .arg(msg.category())
           .arg(fileName)
           .arg(msg.line())
           .arg(msg.function())
           .arg(msg.message());
}

MTK_LOGGER_END_NAMESPACE
