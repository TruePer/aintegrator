////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H

#include "GppLoggerImpl.h"

#include <QDateTime>
#include <QString>



namespace ai {
namespace logger {

class MessageType
{
public:
	enum Enum {
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL
	};
};

struct PWLOGGERLIBSHARED_EXPORT LogMessage
{
	QString shortMessage;
	QString detailedMessage;
	QString sender;
	QDateTime time;
	MessageType::Enum type;

public:
	LogMessage();
	LogMessage(const QString& shortMessage,
	           const QString& detailedMessage = QString(),
	           MessageType::Enum type = MessageType::INFO,
	           const QString& sender = QString());

	QString toString() const;
	QString toShortString() const;
};

}
}

#endif // LOGMESSAGE_H
