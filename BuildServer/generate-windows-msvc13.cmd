@echo off

:: Для задания путей к комилятору, команде make, и прочим необходимым сущностям выполяется файл 3gpp-env-msvc.bat,
:: который помещается на одном уровне с папкой проекта
:: Если необходимо добавление путей к переменной PATH, то их можно добавить в переменную PATH_3GPP.

:: Например
:: call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
:: set path_3gpp=e:\devel\qt\msvc\qt-4.8.3\bin
:: set make_command=e:\devel\qtcreator\qtcreator-3.3.0\bin\jom.exe
::

set SourceDir=%~dp0..

if exist %SourceDir%\..\env-msvc.bat (
	call  %SourceDir%\..\env-msvc.bat
)

if not defined cmake_build_type (
	set cmake_build_type=RELEASE
)

if not defined qt_major_version (
	set qt_major_version=5
)

for %%A in ("%SourceDir%") do set "ProjectDirName=%%~nxA"

set BuildDir=%SourceDir%\%ProjectDirName%-build-msvc13

echo %BuildDir%
if exist %BuildDir% (
	rmdir /s /q %BuildDir%
)

mkdir %BuildDir%

pushd %BuildDir%
cmake -G "Visual Studio 12 2013" -DCMAKE_BUILD_TYPE=%cmake_build_type% -DUSE_QT%qt_major_version%=true %SourceDir% -Wno-dev
::cmake -G "NMake Makefiles" -Wno-dev %SourceDir%
popd

:eof