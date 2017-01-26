////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Application.h"
#include "Translator.h"
#include "Common/Core.h"
#include "Common/Controller.h"

#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QSharedMemory>
#include <QSystemSemaphore>

#include <QMessageBox>

//////////////////////////////////////////////////
static const QString SINGLE_APPLICATION_UNIQUE_NAME      = "AIntegrator unic single apllication name"; /*!< группа настроек оборудования*/
static const QString SINGLE_APPLICATION_UNIQUE_SEMAPHORE = "AIntegrator unic sync name";               /*!< группа настроек оборудования*/
//////////////////////////////////////////////////

void registerMetaTypes();

#if defined(_MSC_VER)
#include <windows.h>
#include <shellapi.h>
int WINAPI WinMain( HINSTANCE /*hInst*/, HINSTANCE /*hPreInst*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ )
{
	int argc;
	char** argv;
	{
		LPWSTR* lpArgv = CommandLineToArgvW( GetCommandLineW(), &argc );
		argv = (char**)malloc( argc*sizeof(char*) );
		int size, i = 0;
		for( ; i < argc; ++i){
			size = wcslen(lpArgv[i]) + 1;
			argv[i] = (char*) malloc(size);
			wcstombs(argv[i], lpArgv[i], size);
		}
		LocalFree(lpArgv);
	}
#else
int main(int argc, char *argv[])
{
#endif

	ai::main::Application app(argc, argv);
	ai::main::Translator translator;
	ai::common::Core core;
	ai::common::Controller controller(&core);

	QSystemSemaphore sema(SINGLE_APPLICATION_UNIQUE_SEMAPHORE, 1);

	// Проверка на единственность запущенного экземпляра
	sema.acquire();
	{
		QSharedMemory shmem(SINGLE_APPLICATION_UNIQUE_NAME);
		shmem.attach();
	}

	QSharedMemory shmem(SINGLE_APPLICATION_UNIQUE_NAME);
	bool isRunning = shmem.attach();
	if(!isRunning) {
		shmem.create(1);
	}
	sema.release();

	if(isRunning) {
		QMessageBox::warning(nullptr, QObject::tr("Warning!"), QObject::tr("Application already started"));
		exit(255);
	}

	app.initialize(core);
	core.initialize();
	registerMetaTypes();

	QQmlApplicationEngine engine;
	engine.addImportPath(":/QmlMaterial");
	engine.rootContext()->setContextProperty("core", &core);
	engine.rootContext()->setContextProperty("controller", &controller);
	engine.rootContext()->setContextProperty("tr", &translator);

	engine.load(QUrl(QStringLiteral("qrc:/MainApplication.qml")));

#ifndef QT_DEBUG
	if(engine.rootObjects().isEmpty()) {
		qCritical("%s", qPrintable(QObject::tr("Can't load qml files")));
		return -1;
	}
#endif

	return app.exec();
}

void registerMetaTypes()
{
	ai::common::Core::registerTypes();
	ai::main::Application::registerTypes();
}
