////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef LOGSTORAGE_H
#define LOGSTORAGE_H

#include <QAbstractListModel>
#include <QList>
#include <QMutex>
#include <QString>
#include <QTimer>

#include "LogMessage.h"
#include "ConsoleLogger.h"
#include "Common/ChangeableReference.h"



namespace ai {
namespace logger {

/**
 * Модель хранилища логов для их отбражения на консоли логов в UI
 */
class Kernel_EXPORT LogStorage : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(quint32 maxRows MEMBER _maxRows NOTIFY maxRowsChanged)

signals:
	void maxRowsChanged();
	void dataUpdated(quint32 count);

public:
	enum Roles {
		ShortMessage = Qt::UserRole + 1,
		Color,
		Time
	};

public:
	LogStorage();

public:
	TSQMessage::SPtr logQueue() const;

public:
	int rowCount(const QModelIndex& index) const;
	QVariant data(const QModelIndex &index, int role) const;
	QHash<int, QByteArray> roleNames() const;

private slots:
	void onTimerTick();

public:
	TSQMessage::SPtr               _logQueue;
	QTimer                         _updateTimer;
	QList<ai::logger::LogMessage>  _data;
	quint32                        _maxRows;
};



}
}
#endif // LOGSTORAGE_H
