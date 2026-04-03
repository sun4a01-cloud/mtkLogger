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
#include <QCoreApplication>
#include <QThread>

#include "mtkLoggerManager.h"
#include "mtkConsoleAppender.h"
#include "mtkFileAppender.h"
#include "mtkRollingFileAppender.h"

using namespace mtk::logger;

// ── Convenience macro: builds a Msg and dispatches via mtkLoggerManager ──────
#define MTK_LOG(level, category, module, message)                         \
    mtkLoggerManager::instance()->log(                                    \
        Msg((level),                                                      \
            QString::fromUtf8(__FILE__),                                  \
            __LINE__,                                                     \
            QString::fromUtf8(__FUNCTION__),                              \
            QStringLiteral(category),                                     \
            (message),                                                    \
            QString::number((quintptr)QThread::currentThreadId()),        \
            QStringLiteral(module)),                                      \
        QStringLiteral(category))

#define MTK_TRACE(cat, mod, msg)   MTK_LOG(Level::Trace,   cat, mod, msg)
#define MTK_DEBUG(cat, mod, msg)   MTK_LOG(Level::Debug,   cat, mod, msg)
#define MTK_INFO(cat, mod, msg)    MTK_LOG(Level::Info,    cat, mod, msg)
#define MTK_WARN(cat, mod, msg)    MTK_LOG(Level::Warning, cat, mod, msg)
#define MTK_ERROR(cat, mod, msg)   MTK_LOG(Level::Error,   cat, mod, msg)
#define MTK_FATAL(cat, mod, msg)   MTK_LOG(Level::Fatal,   cat, mod, msg)

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    auto* mgr = mtkLoggerManager::instance();

    // ── Configure Logger("") — global ───────────────────────────────────────
    mtkLogger* global = mgr->getLogger("");
    global->setLevel(Level::Trace);
    global->addAppender(QSharedPointer<mtkAppender>(
        new mtkConsoleAppender("console")));

    // ── Configure Logger("net") ──────────────────────────────────────────────
    mtkLogger* net = mgr->getLogger("net");
    net->setLevel(Level::Info);
    net->addAppender(QSharedPointer<mtkAppender>(
        new mtkFileAppender("net.log", "net-file")));

    // ── Configure Logger("net.tcp") ──────────────────────────────────────────
    // Inherits "net"'s FileAppender automatically via dot-notation
    mtkLogger* netTcp = mgr->getLogger("net.tcp");
    netTcp->setLevel(Level::Warning);
    netTcp->addAppender(QSharedPointer<mtkAppender>(
        new mtkRollingFileAppender("net_tcp.log", 5 * 1024 * 1024, 3, "net-tcp-rolling")));

    // ── Emit some log messages ───────────────────────────────────────────────

    // Goes to "" console only (level=Trace, "" logger threshold=Trace)
    MTK_TRACE("", "core", "Application started");

    // Goes to "net" file only (level=Info >= "net" threshold=Info)
    MTK_INFO("net", "network", "Network subsystem initialized");

    // Dropped — level=Debug < "net.tcp" threshold=Warning
    MTK_DEBUG("net.tcp", "network", "This message will be dropped");

    // Goes to "net.tcp" rolling file + inherited "net" file
    MTK_WARN("net.tcp", "network", "TCP connection timeout");

    // Goes to "net.tcp" rolling file + inherited "net" file
    MTK_ERROR("net.tcp", "network", "TCP socket error: connection refused");

    return 0;
}
