#ifndef PWLOGGER
#define PWLOGGER

#include <QtCore/qglobal.h>
#include <QString>
#include <QList>
#include <QHash>
#include <QStringList>

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

namespace Log4Qt {
	class Logger;
	class Appender;
	class LogManager;
}

namespace ai {
namespace logger {
class MyLogger;

/**
 * @brief The PwLogger class менеджер логеров
 */
class PWLOGGERLIBSHARED_EXPORT PwLogger
{
public:
	~PwLogger();
	static ai::logger::MyLogger* logger(const QString &name);
	QList<Log4Qt::Appender*> appenders();
	Log4Qt::Appender *appender(const QString &rName);
private:
	static PwLogger *instance();
	/**
	 * @brief initAppenders настройки выводов
	 */
	void initAppenders();
	PwLogger();
	QList<Log4Qt::Appender*>	_appendersList;
	/**
	 * @brief createLogger создадим логер
	 * @param rName
	 * @return
	 */
	ai::logger::MyLogger* createLogger(const QString &rName);
private:
	Log4Qt::Logger *_gppLogger;
	static PwLogger *_pwInstance;
	Log4Qt::LogManager *manager;

	/**
	 * @brief _accessLog если указан здесь лог пишем
	 */
	QStringList	_accessLog;

	/**
	 * @brief _listGppLogger список логеров
	 */
	QHash<QString, MyLogger*> _listGppLogger;
};

} // namespace Logger
}

#endif // PWLOGGER

