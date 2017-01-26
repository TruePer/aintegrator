////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "LogStorage.h"
#include <QDebug>



using namespace ai::logger;


LogStorage::LogStorage()
    : _logQueue(new TSQMessage())
    , _maxRows(50)
{
	connect(&_updateTimer, SIGNAL(timeout()), SLOT(onTimerTick()));
	_updateTimer.start(200);
}

TSQMessage::SPtr LogStorage::logQueue() const
{
	return _logQueue;
}

int LogStorage::rowCount(const QModelIndex& /*index*/) const
{
	return _data.size();
}

QVariant LogStorage::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
		return QVariant();

	auto row = index.row();

	if(row >= _data.count())
		return QVariant();

	auto message = _data[row];

	switch (role) {
	case ShortMessage:
		return message.shortMessage;
	case Color:
		switch(message.type) {
		case ai::logger::MessageType::TRACE:
			return "#444444";
		case ai::logger::MessageType::DEBUG:
			return "blue";
		case ai::logger::MessageType::INFO:
			return "green";
		case ai::logger::MessageType::WARN:
			return "orange";
		case ai::logger::MessageType::ERROR:
			return "red";
		case ai::logger::MessageType::FATAL:
			return "red";

		}
	case Time:
		return message.time.toString("yyyy-MM-dd hh.mm.ss");
	}

	return QVariant();
}

QHash<int, QByteArray> LogStorage::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[ShortMessage] = "ShortMessage";
	roles[Color]        = "MessageColor";
	roles[Time]         = "MessageTime";
	return roles;
}

void LogStorage::onTimerTick()
{
	ai::logger::LogMessage message;

	while(_logQueue->checkEmptyAndDequeue(message)) {

		if(_data.size() > (int)_maxRows) {
			beginRemoveRows(QModelIndex(), 0, 0);
			_data.removeFirst();
			endRemoveRows();
		}

		beginInsertRows(QModelIndex(), _data.count(), _data.count());
		_data << message;
		endInsertRows();

		emit dataUpdated(_data.size());
	}

}
