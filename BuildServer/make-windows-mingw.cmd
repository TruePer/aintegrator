@echo off

:: Для задания путей к комилятору, команде make, и прочим необходимым сущностям выполяется файл 3gpp-env-mingw.bat,
:: который помещается на одном уровне с папкой проекта
:: Если необходимо добавление путей к переменной PATH, то их можно добавить в переменную PATH_3GPP.

:: Например
:: set path_3gpp=c:\QtPlatform\mingw\bin;c:\QtPlatform\qt\mingw\bin
::

set CurrentDir=%~dp0

if exist %CurrentDir%\..\..\3gpp-env-mingw.bat (
	call  %CurrentDir%\..\..\3gpp-env-mingw.bat
)

set path_orig=%path%

set path=%path_3gpp%;%path%

set cmake_generator="MinGW Makefiles"
set make_command=mingw32-make -j%number_of_processors%

call %CurrentDir%\make-windows.cmd

set path=%path_orig%