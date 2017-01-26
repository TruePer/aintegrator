::@echo off

if not defined make_command (
	set make_command=nmake
)

if not defined cmake_generator (
	set cmake_generator="NMake Makefiles"
)

if not defined cmake_build_type (
	set cmake_build_type=RELEASE
)

echo Building project...
echo CMake generator: %cmake_generator%
echo Make command: %make_command%

echo "----- Set environment ---------------------------"
set SourceDir=%~dp0\..

for %%A in ("%SourceDir%") do set "ProjectDirName=%%~nxA"

set BuildDir=%SourceDir%\%ProjectDirName%-build

echo Source Dir: %SourceDir%
echo Build Dir: %BuildDir%

mkdir %BuildDir%
pushd %BuildDir%

echo "----- Building basic configuration ---------------"

cmake -G %cmake_generator% -DCMAKE_BUILD_TYPE=%cmake_build_type% -Wno-dev %SourceDir%
if errorlevel 1 (
	popd
	goto has_error
)

%make_command% all install

if errorlevel 1 (
	popd
	goto has_error
)

if not exist %SourceDir%\Installer (
	mkdir %SourceDir%\Installer
)

popd

echo "----- Creating installer -------------------------"
pushd %SourceDir%\BuildServer
nsis\makensis.exe /V1 Nsis.nsi
popd

if errorlevel 1 (
	popd
	goto has_error
)

echo "----- Building was finished ----------------------"
goto eof

:has_error
  echo  ====== BUILD ERROR!!! ========
:eof
