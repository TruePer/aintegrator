////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CardGroupModel.h"
#include "Protocol/Helper/ProtocolHelper.h"
#include "State/AppState.h"
#include "internal/Ai_Integrator.pb.h"

using namespace ai::main;

CardGroupModel::CardGroupModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int CardGroupModel::rowCount(const QModelIndex& index) const
{
	return _groups.count();
}

QVariant CardGroupModel::data(const QModelIndex& index, int role) const
{
	if (index.row() < 0 || index.row() >= _groups.size())
		return QVariant();

	switch (role) {
	case Group:
		return _groups.at(index.row());
	case Count:
		return _groups.count();
	}

	return QVariant();
}

QHash<int, QByteArray> CardGroupModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Group] = "group";
	roles[Count] = "count";
	return roles;
}

void CardGroupModel::update(const ai::state::AppState* state)
{
	auto mgList = ai::protocol::ProtocolHelper::moduleGroups(*state->integratorOptions(), _workStation);

	for(const auto& mg : mgList) {
		auto name = QString::fromStdString(mg.name());

		if(!_groups.contains(name)) {
			beginInsertRows(QModelIndex(), _groups.count(), _groups.count());

			_groups << name;

			endInsertRows();
		}
	}
}

void CardGroupModel::setWorkStation(const QString& ws)
{
	if(_workStation == ws)
		return;

	_workStation = ws;
	beginResetModel();
	_groups.clear();
	endResetModel();
}

