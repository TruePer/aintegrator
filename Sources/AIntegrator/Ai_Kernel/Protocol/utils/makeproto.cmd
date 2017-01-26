@echo off

set CURRENT_PATH=%~dp0

rem Directories
set PROTO_PATH=%CURRENT_PATH%..\proto
set CPP_PATH=%CURRENT_PATH%..\code\proto

rem Protobuf generator
set PROTOC=%CURRENT_PATH%..\bin\protoc.exe

if exist %CPP_PATH% (
	rmdir /s /q %CPP_PATH%
)

mkdir %CPP_PATH%

rem Теперь сгенерим исходники
For %%f In (%PROTO_PATH%\host\*) Do (
	rem Печатаем имя файла
	echo %%~nf
	call  %PROTOC% --cpp_out=dllexport_decl=LIBPROTOBUF_EXPORT:%CPP_PATH% --proto_path=%PROTO_PATH% %%f
)


For %%f In (%PROTO_PATH%\internal\*) Do (
	rem Печатаем имя файла
	echo %%~nf
	call  %PROTOC% --cpp_out=dllexport_decl=LIBPROTOBUF_EXPORT:%CPP_PATH% --proto_path=%PROTO_PATH% %%f
)
