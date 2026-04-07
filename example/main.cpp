#include "mtkLoggerManager.h"
#include "mtkLoggerMessage.h"
#include <mtkLoggerMacro.h>
#include <Appender/mtkConsoleAppender.h>

int main() {
    mtk::logger::LoggerManager* loggerManager = mtk::logger::LoggerManager::instance();
    mtk::logger::AbstractLogger* logger = loggerManager->getLogger("");
    logger->addAppender(QSharedPointer<mtk::logger::ConsoleAppender>::create());


    LOG_WARNING() << "Hello, mtkLogger!";
    return 0;
}