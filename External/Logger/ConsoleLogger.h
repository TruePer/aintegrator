////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_LOGGER_CONSOLELOGGER_H
#define AI_LOGGER_CONSOLELOGGER_H

#include "GppLoggerImpl.h"
#include "Common/ThreadSaveQueue.h"
#include "LogMessage.h"



typedef ai::common::ThreadSaveQueue<ai::logger::LogMessage> TSQMessage;

namespace ai {
namespace logger {

class PWLOGGERLIBSHARED_EXPORT ConsoleLogger : public MyLogger
{
public:
	ConsoleLogger(Log4Qt::Logger *logger);

public:
	void debug(const QString& rMessage) const;
	void error(const QString& rMessage) const;
	void fatal(const QString& rMessage) const;
	void info(const QString& rMessage) const;
	void trace(const QString& rMessage) const;
	void warn(const QString& rMessage) const;

public:
	void registerStorage(TSQMessage::SPtr queue);

private:
	TSQMessage::SPtr _queue;
};

} // namespace logger
} // namespace ai

#endif // AI_LOGGER_CONSOLELOGGER_H
