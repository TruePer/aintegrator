#ifndef PWLOGGERLIB_H
#define PWLOGGERLIB_H

#include <QtCore/qglobal.h>
#include <QString>

#ifdef PWLOGGERLIB_BUILD
 #if (defined(QT_DLL) || defined(QT_SHARED)) && !defined(QT_PLUGIN)
   // lib being compiled shared
  #define PWLOGGERLIBSHARED_EXPORT Q_DECL_EXPORT
 #else
  // lib being embedded
  #define PWLOGGERLIBSHARED_EXPORT
 #endif
#else
 // lib being linked against (must be shared on Window$!)
 #define PWLOGGERLIBSHARED_EXPORT Q_DECL_IMPORT
#endif


namespace Logger
{

class PWLOGGERLIBSHARED_EXPORT PwLoggerConfigure
{
public:
	PwLoggerConfigure(const QString &configFile);
	~PwLoggerConfigure();
};

class ILogger {
public:
	virtual void debug(const QString & msg) = 0;
	virtual void error(const QString & msg) = 0;
	virtual void warn(const QString & msg) = 0;
	virtual void fatal(const QString & msg) = 0;
	virtual void info(const QString & msg) = 0;
	virtual void debugFormat(const char* format, ...) = 0;
	virtual void errorFormat(const char* format, ...) = 0;
	virtual void warnFormat(const char* format, ...) = 0;
	virtual void fatalFormat(const char* format, ...) = 0;
	virtual void infoFormat(const char* format, ...) = 0;
};

#define LOGGERCLASSNAME( x )   #x

class PWLOGGERLIBSHARED_EXPORT PwLoggerFactory
{
public:
	enum LoggerTypeEnum
	{
		Log4Qt
	};
private:
	static PwLoggerFactory* _instance;
	LoggerTypeEnum _defaultLoggerType;
	PwLoggerFactory();
public:
	static PwLoggerFactory* Instance();
	static void destroy();
	void Configure(const QString &config_file);
	ILogger* createLogger(const QString &name);
	ILogger* createLogger(LoggerTypeEnum loggerType, const QString &name);
};

} // namespace Logger

#endif
