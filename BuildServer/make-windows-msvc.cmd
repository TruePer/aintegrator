@echo off

:: Для задания путей к комилятору, команде make, и прочим необходимым сущностям выполяется файл env-msvc.bat,
:: который помещается на одном уровне с папкой проекта
:: Если необходимо добавление путей к переменной PATH, то их можно добавить в переменную MY_PATH.

:: Например
:: call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
:: set path_3gpp=e:\devel\qt\msvc\qt-4.8.3\bin
:: set make_command=e:\devel\qtcreator\qtcreator-3.3.0\bin\jom.exe
::

set CurrentDir=%~dp0

set path_orig=%path%

if exist %CurrentDir%\..\..\env-msvc.bat (
	call  %CurrentDir%\..\..\env-msvc.bat
)

set path=%MY_PATH%;%path%

set cmake_generator="NMake Makefiles"

if not defined make_command (
	set make_command=nmake
)

call %CurrentDir%\make-windows.cmd

set path=%path_orig%