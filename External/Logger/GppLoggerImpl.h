#ifndef GPP_LOGGER_IMPL_H
#define GPP_LOGGER_IMPL_H

#include "log4qt/logger.h"

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

namespace ai {
namespace logger {

PWLOGGERLIBSHARED_EXPORT const QString registerLog( const char* );
PWLOGGERLIBSHARED_EXPORT const QStringList getLogSections();

/**
 *	обертка над logger
 */
class PwLogger;

class PWLOGGERLIBSHARED_EXPORT MyLogger
{
public:
	explicit MyLogger(Log4Qt::Logger *logger);
	virtual ~MyLogger();

	virtual void debug(const QString &rMessage) const;
	virtual void error(const QString &rMessage) const;
	virtual void fatal(const QString &rMessage) const;
	virtual void info(const QString &rMessage) const;
	virtual void trace(const QString &rMessage) const;
	virtual void warn(const QString &rMessage) const;

private:
	Log4Qt::Logger *_logger;
	friend class PwLogger;
};

}

///
/// \brief GppLoggerStream - помощник вывода отладочной информации
///
class PWLOGGERLIBSHARED_EXPORT GppLoggerStream {

	GppLoggerStream( const GppLoggerStream& );

public:
	GppLoggerStream( const QString& name );

	inline GppLoggerStream& operator <<( const QString& value ) {
		if( !debugString_.isEmpty() && !debugString_.endsWith(L'\n') )
			debugString_.append( L' ' );
		debugString_.append( value );
		return *this;
	}

	template< class T >
	inline GppLoggerStream& operator <<( const T& value ) {
		return this->operator <<( QString( "%1" ).arg( value ) );
	}

protected:
	logger::MyLogger* logger_;
	QString debugString_;
};

static const QString gppLog_log    = logger::registerLog( "log" ); // Логгер по умолчанию

// log оно же info
class PWLOGGERLIBSHARED_EXPORT log : public GppLoggerStream {
public:
	inline log( const QString& name = gppLog_log ) : GppLoggerStream( name ){}
	inline ~log()  {	logger_->trace( debugString_ ); }
};

// trace
class PWLOGGERLIBSHARED_EXPORT info : public GppLoggerStream {
public:
	inline info( const QString& name = gppLog_log ) : GppLoggerStream( name ){}
	inline ~info() {	logger_->info( debugString_ ); }
};

// debug
class PWLOGGERLIBSHARED_EXPORT debug : public GppLoggerStream {
public:
	inline debug( const QString& name = gppLog_log ) : GppLoggerStream( name ){}
	inline ~debug() {	logger_->debug( debugString_ ); }
};

// warn
class PWLOGGERLIBSHARED_EXPORT warn : public GppLoggerStream {
public:
	inline warn( const QString& name = gppLog_log ) : GppLoggerStream( name ){}
	inline ~warn() {	logger_->warn( debugString_ ); }
};

// error
class PWLOGGERLIBSHARED_EXPORT error : public GppLoggerStream {
public:
	inline error( const QString& name = gppLog_log ) : GppLoggerStream( name ){}
	inline ~error() {	logger_->error( debugString_ ); }
};

// fatal
class PWLOGGERLIBSHARED_EXPORT fatal : public GppLoggerStream {
public:
	inline fatal( const QString& name = gppLog_log ) : GppLoggerStream( name ){}
	inline ~fatal() {	logger_->fatal( debugString_ ); }
};

}

#endif // GPP_LOGGER_IMPL_H
