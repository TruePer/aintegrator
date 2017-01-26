////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MainWindow.h"

#include <QApplication>
#include <QTextCodec>

#if defined(_MSC_VER)
#include <io.h>
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
			memcpy(argv[i], QString::fromUtf16((const ushort*) lpArgv[i]).toLocal8Bit().data(),size);
		}
		LocalFree(lpArgv);
	}
#else
int main(int argc, char* argv[])
{
#endif
	QApplication app(argc, argv);

	QString moduleId;
	if(argc > 1) {
		moduleId = argv[1];
	}

	// Установка кодеков
#ifdef Q_OS_WIN32
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp1251"));
#else
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("Utf8"));
#endif

	MainWindow mainWindow(moduleId);
	mainWindow.show();
	return app.exec();
}
