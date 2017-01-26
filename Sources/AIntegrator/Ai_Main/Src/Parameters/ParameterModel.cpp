////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "GroupName.h"
#include "Parameter.h"
#include "ParameterModel.h"
#include "Command/Transaction/SetPostParameters.h"
#include "internal/Ai_Integrator.pb.h"
#include "Protocol/Helper/ProtocolHelper.h"
#include "State/AppState.h"

#include <QDebug>



using namespace ai::main;

ParameterModel::ParameterModel(QObject* parent)
    : QAbstractListModel(parent)
    , _appState(nullptr)
    , _isOpenedInUi(false)
{
}

int ParameterModel::rowCount(const QModelIndex& /*index*/) const
{
	return _parameterIds.size();
}

QVariant ParameterModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
		return QVariant();

	auto row = index.row();

	if(row >= _parameterIds.count())
		return QVariant();

	auto id = _parameterIds[row];

	switch (role) {
	case ParameterItem:
		return QVariant::fromValue(_parameters[id]);
	}

	return QVariant();
}

QHash<int, QByteArray> ParameterModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[ParameterItem] = "_parameter";
	return roles;
}

QString ParameterModel::executionModuleId() const
{
	return _executionModuleId;
}

void ParameterModel::setExecutionModuleId(QString executionModuleId)
{
	if (_executionModuleId == executionModuleId)
		return;

	beginResetModel();

	for(auto* parameter : _parameters) {
		parameter->deleteLater();
	}
	_parameters.clear();
	_parameterIds.clear();

	endResetModel();

	_executionModuleId = executionModuleId;
	emit executionModuleIdChanged(executionModuleId);
}

ai::proto::PostParameters ParameterModel::moduleParameters() const
{
	return _protoParameters;
}

/**
 * Применить текущие параметры
 */
void ParameterModel::apllyParameters()
{
	auto command = ai::command::ICommand::SPtr(new ai::command::SetPostParameters(_executionModuleId, _protoParameters));
	emit sendCommand(command);
}

/**
 * Модель открыта для просмотра в UI
 */
void ParameterModel::opened()
{
	_isOpenedInUi = true;
}

/**
 * Просмотр модели в UI завершен
 */
void ParameterModel::closed()
{
	_isOpenedInUi = false;
}

bool ParameterModel::valid() const
{
	return _valid;
}

void ParameterModel::setValid(bool valid)
{
	if (_valid == valid)
		return;

	_valid = valid;
	emit validChanged();
}

void ParameterModel::update(const ai::state::AppState* state)
{
	_appState = state;

	auto protoParameters = protocol::ProtocolHelper::executionModuleParameter(state->integratorOptions(), _executionModuleId.toStdString());
	setValid(protoParameters.groups_size());

	if(_isOpenedInUi && _executionModuleId == _prevExecutionModuleId)
		return;

	_protoParameters = protoParameters;

	_prevExecutionModuleId = _executionModuleId;

	for(int i = 0, size = _protoParameters.groups_size(); i < size; ++i) {
		auto* group = _protoParameters.mutable_groups(i);

		auto id = parameterId(group->name(), "group");

		if(!_parameters.contains(id)) {
			beginInsertRows(QModelIndex(), _parameterIds.count(), _parameterIds.count());

			auto* groupInfo = new GroupName(this);
			groupInfo->setName(QString::fromStdString(group->name()));
			_parameters.insert(id, groupInfo);
			_parameterIds << id;

			endInsertRows();
		}

		for(int i = 0, size = group->parameters_size(); i < size; ++i) {
			auto* param = group->mutable_parameters(i);
			auto id = parameterId(group->name(), param->id());

			if(!_parameters.contains(id)) {
				beginInsertRows(QModelIndex(), _parameterIds.count(), _parameterIds.count());

				auto* parameterInfo = new Parameter(this);
				_parameters.insert(id, parameterInfo);
				_parameterIds << id;

				endInsertRows();
			}

			auto* parameterInfo = _parameters[id];
			parameterInfo->update(param);
		}
	}
}

QString ParameterModel::parameterId(const std::string& groupName, const std::string& parameterId)
{
	return QString("%1-%2").arg(QString::fromStdString(groupName)).arg(QString::fromStdString(parameterId));
}
