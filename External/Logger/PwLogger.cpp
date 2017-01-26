#include "ConsoleLogger.h"
#include "PwLogger.h"
#include "log4qt/logmanager.h"
#include "log4qt/appender.h"
#include "log4qt/consoleappender.h"
#include "log4qt/ttcclayout.h"
#include "log4qt/patternlayout.h"
#include "log4qt/varia/levelrangefilter.h"
#include "log4qt/varia/denyallfilter.h"
#include "log4qt/varia/debugappender.h"

#include "Logger.h"

#include <QSettings>
#include <QDebug>


using namespace ai::logger;
using namespace Log4Qt;

//QList<Appender*> PwLogger::AppendersList;
PwLogger* PwLogger::_pwInstance = NULL;

PwLogger::~PwLogger()
{
	//delete _mpLoggerRepository;
}

MyLogger *PwLogger::logger(const QString &name)
{
	return instance()->createLogger(name);
}

QList<Appender *> PwLogger::appenders()
{
	return _appendersList;
}

Appender* PwLogger::appender(const QString &rName)
{
	QListIterator<Appender *> it(_appendersList);
	while(it.hasNext())
	{
		Appender *out = it.next();
		if(out->name() == rName)
		{
			return out;
		}
	}
	return NULL;
}

void PwLogger::initAppenders()
{
	LevelRangeFilter *p_filter = NULL;
	// Common deny all filter
	Filter *p_denyall = new DenyAllFilter();
	p_denyall->activateOptions();

	PatternLayout *debugLayout = new PatternLayout( "%-4t %-12c [%-5p] %m\n" );
#ifdef Q_OS_WIN
	AppenderSkeleton *consoleOut = new DebugAppender(debugLayout);
#else
	AppenderSkeleton *consoleOut = new ConsoleAppender(debugLayout, ConsoleAppender::STDOUT_TARGET);
#endif
	consoleOut->setName("console");
	p_filter = new LevelRangeFilter();
	p_filter->setNext(p_denyall);
	p_filter->setLevelMin(Level::NULL_INT);
	p_filter->setLevelMax(Level::INFO_INT);
	p_filter->activateOptions();
	consoleOut->addFilter(p_filter);
	consoleOut->activateOptions();
	_appendersList.append(consoleOut);

// второй для вывода ошибок
	PatternLayout *errLayout = new PatternLayout( "%-4t %-12c [%-5p] %m\n" );
#ifdef Q_OS_WIN
	AppenderSkeleton *consoleErr = new DebugAppender(errLayout);
#else
	AppenderSkeleton *consoleErr = new ConsoleAppender(errLayout, ConsoleAppender::STDERR_TARGET);
#endif
	consoleErr->setName("consoleErr");
	p_filter = new LevelRangeFilter();
	p_filter->setNext(p_denyall);
	p_filter->setLevelMin(Level::WARN_INT);
	p_filter->setLevelMax(Level::OFF_INT);
	p_filter->activateOptions();

	consoleErr->addFilter(p_filter);
	consoleErr->activateOptions();
	_appendersList.append(consoleErr);
}

PwLogger::PwLogger()
    : _gppLogger(NULL)
{
		QSettings s;
		s.beginGroup("LogSettings");
		_accessLog = s.value("logAccess").toStringList();
		s.endGroup();
}

MyLogger* PwLogger::createLogger(const QString &rName)
{
	MyLogger *p_logger = _listGppLogger.value(QString("ai::") + rName, 0);
	if(p_logger != 0) {
		return p_logger;
	}
	else {
		p_logger = new ConsoleLogger(manager->logger(QString("ai::") + rName));

		Log4Qt::Level defaultLevel = Log4Qt::Level::DEBUG_INT;
		Log4Qt::Level sectionLevel = Log4Qt::Level::ALL_INT;


		p_logger->_logger->setLevel(defaultLevel);
		if(instance()->_accessLog.contains(rName))
			p_logger->_logger->setLevel(sectionLevel);

		_listGppLogger.insert(p_logger->_logger->name(), p_logger);
		return p_logger;
	}
}

PwLogger *PwLogger::instance()
{
	if(!_pwInstance)
	{
		_pwInstance = new PwLogger();
		_pwInstance->manager = Log4Qt::LogManager::instance();
		_pwInstance->_gppLogger = Log4Qt::LogManager::logger("ai");
		_pwInstance->initAppenders();

		Appender *out = _pwInstance->appender("console");
		Q_ASSERT(out);
		_pwInstance->_gppLogger->addAppender(out);
		Appender *err = _pwInstance->appender("consoleErr");
		Q_ASSERT(err);
		_pwInstance->_gppLogger->addAppender(err);

		// сообщения ошибок выводить в консоль
		Log4Qt::LogManager::instance()->qtLogger()->addAppender(out);
		Log4Qt::LogManager::instance()->qtLogger()->addAppender(err);
	}
	return _pwInstance;
}
