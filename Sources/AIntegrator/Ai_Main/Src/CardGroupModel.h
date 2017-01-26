////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_MAIN_CARDGROUPMODEL_H
#define AI_MAIN_CARDGROUPMODEL_H

#include "Common/IObserver.h"
#include <QAbstractListModel>



namespace ai {
namespace main {

/**
 * Модель для представления групп запускемых модлей на UI
 */
class CardGroupModel : public QAbstractListModel, public common::IObserver
{
	Q_OBJECT
	Q_PROPERTY(QString workStation MEMBER _workStation WRITE setWorkStation)

signals:
	void sendCommand(const ai::command::ICommand::SPtr command);

public:
	enum Roles {
		Group =  Qt::UserRole + 1,
		Count
	};

public:
	CardGroupModel(QObject* parent = nullptr);

	int rowCount(const QModelIndex& index) const;
	QVariant data(const QModelIndex &index, int role) const;
	QHash<int, QByteArray> roleNames() const;

private slots:
	void update(const ai::state::AppState* state);

private:
	Q_INVOKABLE void setWorkStation(const QString& ws);

private:
	QStringList                    _groups;
	QString                        _workStation;

};

} // namespace main
} // namespace ai

#endif // AI_MAIN_CARDGROUPMODEL_H
