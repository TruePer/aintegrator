////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ConsoleLogger.h"



using namespace ai::logger;

ConsoleLogger::ConsoleLogger(Log4Qt::Logger* logger)
    : MyLogger(logger)
{
}

void ConsoleLogger::debug(const QString& rMessage) const
{
	MyLogger::debug(rMessage);
	if(_queue) {
		ai::logger::LogMessage log;
		log.type = ai::logger::MessageType::DEBUG;
		log.shortMessage = rMessage;

		_queue->enqueue(log);
	}
}

void ConsoleLogger::error(const QString& rMessage) const
{
	MyLogger::debug(rMessage);
	if(_queue) {
		ai::logger::LogMessage log;
		log.type = ai::logger::MessageType::ERROR;
		log.shortMessage = rMessage;

		_queue->enqueue(log);
	}
}

void ConsoleLogger::fatal(const QString& rMessage) const
{
	MyLogger::debug(rMessage);
	if(_queue) {
		ai::logger::LogMessage log;
		log.type = ai::logger::MessageType::FATAL;
		log.shortMessage = rMessage;

		_queue->enqueue(log);
	}
}

void ConsoleLogger::info(const QString& rMessage) const
{
	MyLogger::debug(rMessage);
	if(_queue) {
		ai::logger::LogMessage log;
		log.type = ai::logger::MessageType::INFO;
		log.shortMessage = rMessage;

		_queue->enqueue(log);
	}
}

void ConsoleLogger::trace(const QString& rMessage) const
{
	MyLogger::debug(rMessage);
	if(_queue) {
		ai::logger::LogMessage log;
		log.type = ai::logger::MessageType::TRACE;
		log.shortMessage = rMessage;

		_queue->enqueue(log);
	}
}

void ConsoleLogger::warn(const QString& rMessage) const
{
	MyLogger::debug(rMessage);
	if(_queue) {
		ai::logger::LogMessage log;
		log.type = ai::logger::MessageType::TRACE;
		log.shortMessage = rMessage;

		_queue->enqueue(log);
	}
}

void ConsoleLogger::registerStorage(TSQMessage::SPtr queue)
{
	_queue = queue;
}
