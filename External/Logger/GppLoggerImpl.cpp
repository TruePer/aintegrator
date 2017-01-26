#include "GppLoggerImpl.h"

#include "PwLogger.h"

namespace ai {
namespace logger {

QStringList& staticSections() {
	static QStringList list;
	return list;
}

const QString registerLog( const char* section ) {

	QString result{ section };

	if( !staticSections().contains( result ) )
		staticSections().append( result );

	return result;
}

const QStringList getLogSections()
{
	return staticSections();
}

MyLogger::MyLogger(Log4Qt::Logger *logger) : _logger(logger)
{
	Q_ASSERT(_logger);
}

MyLogger::~MyLogger()
{
}
void MyLogger::debug(const QString &rMessage) const
{	_logger->debug(rMessage);}

void MyLogger::error(const QString &rMessage) const
{	_logger->error(rMessage);}

void MyLogger::fatal(const QString &rMessage) const
{	_logger->fatal(rMessage);}

void MyLogger::info(const QString &rMessage) const
{	_logger->info(rMessage);}

void MyLogger::trace(const QString &rMessage) const
{	_logger->trace(rMessage);}

void MyLogger::warn(const QString &rMessage) const
{ _logger->warn(rMessage);}

}

GppLoggerStream::GppLoggerStream( const QString& name )
	: logger_( logger::PwLogger::logger( name ) )
{

}
}
