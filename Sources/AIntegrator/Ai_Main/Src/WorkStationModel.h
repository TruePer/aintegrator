////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_MAIN_WORKSTATIONMODEL_H
#define AI_MAIN_WORKSTATIONMODEL_H

#include "Common/IObserver.h"
#include <QAbstractListModel>



namespace ai {
namespace main {

/**
 * Модель для отображения на UI списка АРМ
 */
class WorkStationModel : public QAbstractListModel, public common::IObserver
{
	Q_OBJECT
signals:
	void sendCommand(const ai::command::ICommand::SPtr command);

public:
	enum Roles {
		WorkStation = Qt::UserRole + 1
	};

public:
	WorkStationModel(QObject* parent = nullptr);

public:
	int rowCount(const QModelIndex& index) const;
	QVariant data(const QModelIndex &index, int role) const;
	QHash<int, QByteArray> roleNames() const;

private slots:
	void update(const ai::state::AppState* state);

private:
	QList<QString>                 _workStations;
	const ai::state::AppState*     _appState;
};

} // namespace main
} // namespace ai

#endif // AI_MAIN_WORKSTATIONMODEL_H
