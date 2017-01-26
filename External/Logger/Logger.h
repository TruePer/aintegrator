#ifndef MY_LOGGER_H
#define MY_LOGGER_H

#include "GppLoggerImpl.h"

namespace ai {

/// Использование:
/// ai::log  ( [name] ) << "debug information";
/// ai::trace( [name] ) << "debug information";
/// ai::debug( [name] ) << "debug information";
/// ai::warn ( [name] ) << "debug information";
/// ai::error( [name] ) << "debug information";
/// ai::fatal( [name] ) << "debug information";
///
/// где [name] - необязательное имя группы, по умолчанию подставляется "log"
///
/// Для структурирования отладочной информации, требуется в каждом плагине использовать
/// собственное имя.
/// Для команд и общего модуля для удобства введены синонимы:
/// logCommand, logCommon

// Константы для определения группы отладочной информации
const QString gppLog_command    = logger::registerLog( "command" );     // Логгер команд
const QString gppLog_protobuf   = logger::registerLog( "proto" );       // Логгер protobuf-сообщений

// Логгер команд
class PWLOGGERLIBSHARED_EXPORT logCommand : public log {
public:
	inline logCommand() : log( gppLog_command ){}
};

// Логгер protobuf-сообщений
class PWLOGGERLIBSHARED_EXPORT logProtobuf : public log {
public:
	inline logProtobuf() : log( gppLog_protobuf ){}
};

}

#endif // MY_LOGGER_H
