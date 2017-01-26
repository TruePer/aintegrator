////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef AI_MAIN_PARAMETERMODEL_H
#define AI_MAIN_PARAMETERMODEL_H

#include "Common/IObserver.h"
#include "host/Ai_PostParameters.pb.h"
#include <QAbstractListModel>



namespace ai {
namespace main {

class BaseParameter;

class ParameterModel : public QAbstractListModel, public common::IObserver
{
	Q_OBJECT

	Q_PROPERTY(QString executionModuleId READ executionModuleId WRITE setExecutionModuleId NOTIFY executionModuleIdChanged)
	Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged)

public:
	enum Roles {
		ParameterItem = Qt::UserRole + 1
	};

signals:
	void executionModuleIdChanged(QString executionModuleId);
	void validChanged();
	void sendCommand(const ai::command::ICommand::SPtr command);

public:
	ParameterModel(QObject* parent = nullptr);

public:
	int rowCount(const QModelIndex& index) const;
	QVariant data(const QModelIndex &index, int role) const;
	QHash<int, QByteArray> roleNames() const;

	QString executionModuleId() const;
	void setExecutionModuleId(QString executionModuleId);

	bool valid() const;
	void setValid(bool valid);

	Q_INVOKABLE ai::proto::PostParameters moduleParameters() const;

	Q_INVOKABLE void apllyParameters();
	Q_INVOKABLE void opened();
	Q_INVOKABLE void closed();

private slots:
	void update(const ai::state::AppState* state);

private:
	static QString parameterId(const std::string& groupName, const std::string& parameterId);

private:
	const ai::state::AppState*        _appState;              // Состояние приложения (константный указатель - тут менть нельзя)
	ai::proto::PostParameters         _protoParameters;       // Копия параметров из настроек модуля
	QHash<QString, BaseParameter*>    _parameters;            // Проиндексированные параметры (для отображения на модели)
	QList<QString>                    _parameterIds;          // Индекся параметров. Используется для задания порядка в модели
	QString                           _executionModuleId;
	QString                           _prevExecutionModuleId; //
	bool                              _isOpenedInUi;          // Признак того, что данные модели показываются на UI
	bool                              _valid;
};

} // namespace main
} // namespace ai

#endif // AI_MAIN_PARAMETERMODEL_H
