////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Application.h"
#include "ConsoleLogger.h"
#include "Logger.h"
#include "PwLogger.h"
#include "Common/Core.h"
#include "State/AppState.h"

#include <QLocale>
#include <QSettings>
#include <QThread>
#include <QTextCodec>
#include <QTranslator>



using namespace ai::main;

QString header = QString("\n============================================ \r\n"
                          "\t Запуск приложения \r\n"
						  "============================================ \r\n");

Application::Application(int& argc, char** argv)
    : QApplication(argc, argv)
    , _translator(nullptr)
{
	setApplicationName("AIntegrator");
	setOrganizationDomain("nppntt");
	thread()->setObjectName("MAIN");

	// Установка кодеков
#ifdef Q_OS_WIN32
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp1251"));
#else
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("Utf8"));
#endif

	setupLog4Qt();
}

void Application::initialize(ai::common::Core& core)
{
	// Инициализируем логгер.
	auto* logger = ai::logger::PwLogger::logger("log");

	ai::logger::ConsoleLogger* consoleLogger = dynamic_cast<ai::logger::ConsoleLogger*>(logger);

	if(consoleLogger) {
		consoleLogger->registerStorage(core.state()->logStorage().logQueue());
	}
}

#include "CardGroupModel.h"
#include "CardModel.h"
#include "Parameters/CheckBoxParameter.h"
#include "Parameters/GroupName.h"
#include "Parameters/IntervalParameter.h"
#include "Parameters/LineEditParameter.h"
#include "Parameters/Parameter.h"
#include "Parameters/ParameterModel.h"
#include "Parameters/SelectListParameter.h"
#include "WorkStationModel.h"
#include "CardState/SimpleCard.h"
#include <QtQml>

void Application::registerTypes()
{
	qmlRegisterType<ai::main::CardGroupModel>     ("ai.main", 1, 0, "CardGroupModel");
	qmlRegisterType<ai::main::CardModel>          ("ai.main", 1, 0, "CardModel");
	qmlRegisterType<ai::main::ParameterModel>     ("ai.main", 1, 0, "ParameterModel");
	qmlRegisterType<ai::main::SimpleCard>         ("ai.main", 1, 0, "SimpleCard");
	qmlRegisterType<ai::main::StatusIcon>         ("ai.main", 1, 0, "StatusIcon");
	qmlRegisterType<ai::main::WorkStationModel>   ("ai.main", 1, 0, "WorkStationModel");

	qmlRegisterType<ai::main::Parameter>          ("ai.main", 1, 0, "Parameter");
	qmlRegisterType<ai::main::GroupName>          ("ai.main", 1, 0, "GroupName");
	qmlRegisterType<ai::main::CheckBoxParameter>  ("ai.main", 1, 0, "CheckBoxParameter");
	qmlRegisterType<ai::main::LineEditParameter>  ("ai.main", 1, 0, "LineEditParameter");
	qmlRegisterType<ai::main::SelectListParameter>("ai.main", 1, 0, "SelectListParameter");
	qmlRegisterType<ai::main::IntervalParameter>  ("ai.main", 1, 0, "IntervalParameter");

	qmlRegisterType<ai::logger::LogStorage>       ("ai.main", 1, 0, "LogStorage");
}

void Application::setupLog4Qt()
{
	QApplication::setApplicationName("AIntegrator");
	QApplication::setOrganizationDomain("nppntt");

	QSettings settings;
	settings.beginGroup("Log4Qt"); {
		settings.beginGroup("Properties"); {
			settings.setValue("log4j.appender.file", "org.apache.log4j.RollingFileAppender");
			settings.setValue("log4j.appender.file.file", state::AppState::settingsDirectory() + "/log/integrator.log");
			settings.setValue("log4j.appender.file.AppendFile", "true");
			settings.setValue("log4j.appender.file.MaxFileSize", "10MB");
			settings.setValue("log4j.appender.file.MaxBackupIndex", "20");
			settings.setValue("log4j.appender.file.layout", "org.apache.log4j.PatternLayout");
			settings.setValue("log4j.appender.file.layout.conversionPattern", "%d{dd-MM-yyyy HH:mm:ss.zzz} [%-5p] %c: %m%n");
			settings.setValue("log4j.appender.file.layout.header", header);
#if defined(Q_WS_WIN)
			settings.setValue("log4j.appender.debug", "org.apache.log4j.varia.DebugAppender");
			settings.setValue("log4j.appender.debug.layout", "org.apache.log4j.PatternLayout");
			settings.setValue("log4j.appender.debug.layout.conversionPattern", "%d{HH:mm:ss.zzz} [%-5p] %c: %m\n");
#endif
			settings.setValue("log4j.handleQtMessages", "1");
			settings.setValue("log4j.rootLogger", "ALL, file"

#if defined(Q_WS_WIN)
										   ", debug"
#endif
			);
			settings.endGroup(); // Properties
		}
		settings.endGroup(); // Log4Qt
	}


	settings.beginGroup("LogSettings"); {
		QStringList availableLogs;
		availableLogs
		        << "log"
		        << "command"
		        << "proto"
		           ;
		settings.setValue("logAccess", availableLogs);
		settings.endGroup();
	}
}
