////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "WorkStationModel.h"

#include "internal/Ai_Integrator.pb.h"
#include "State/AppState.h"



using namespace ai::main;

WorkStationModel::WorkStationModel(QObject* parent)
    : QAbstractListModel(parent)
    , _appState(nullptr)
{
}

int WorkStationModel::rowCount(const QModelIndex& /*index*/) const
{
	return _workStations.count();
}

QVariant WorkStationModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
		return QVariant();

	auto row = index.row();

	if(row >= _workStations.count())
		return QVariant();

	switch (role) {
	case WorkStation:
		return QVariant::fromValue(_workStations[row]);
	}

	return QVariant();
}

QHash<int, QByteArray> WorkStationModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[WorkStation] = "_workStation";
	return roles;
}

void WorkStationModel::update(const ai::state::AppState* state)
{
	_appState = state;

	const auto* integrator = _appState->integratorOptions();

	for(int i =0, size = integrator->work_stations_size(); i < size; ++i) {
		const auto workStationName = QString::fromStdString(integrator->work_stations(i).name());

		if(!_workStations.contains(workStationName)) {
			beginInsertRows(QModelIndex(), _workStations.count(), _workStations.count());
			_workStations << workStationName;
			endInsertRows();
		}
	}
}
