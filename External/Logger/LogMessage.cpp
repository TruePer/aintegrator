////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "LogMessage.h"

#include <QDateTime>
#include <QStringList>

using namespace ai::logger;

QStringList types = QString("TRACE,DEBUG,INFO   ,WARN ,ERROR,FATAL  ").split(",");

LogMessage::LogMessage()
    : time(QDateTime::currentDateTime())
    , type(MessageType::INFO)
{
}

LogMessage::LogMessage(const QString& shortMessage, const QString& detailedMessage, MessageType::Enum type, const QString& sender)
    : shortMessage(shortMessage)
    , detailedMessage(detailedMessage)
    , sender(sender)
    , time(QDateTime::currentDateTime())
    , type(type)
{
}

QString LogMessage::toString() const
{
	return QString("[%1] - %2 - %3%4")
			.arg(time.toString("dd.MM.yy hh-mm-ss.zzz"))
			.arg(types.at(type))
			.arg(QString(sender.isEmpty() ? "" : sender+": "))
			.arg(shortMessage);
}

QString LogMessage::toShortString() const
{


	return QString("[%1] - %2 - %3%4")
			.arg(time.toString("hh-mm-ss"))
			.arg(types.at(type))
			.arg(QString(sender.isEmpty() ? "" : sender+": "))
			.arg(shortMessage);
}

