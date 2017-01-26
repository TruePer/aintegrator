////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_MAIN_BASECARD_H
#define AI_MAIN_BASECARD_H

#include "internal/Ai_ExecutionModule.pb.h"

#include <QHash>
#include <QObject>



namespace ai {
namespace main {

class BaseCard : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString     qmlPath            MEMBER _qmlPath            NOTIFY qmlPathChanged)
	Q_PROPERTY(QString     name               MEMBER _name               NOTIFY nameChanged)
	Q_PROPERTY(QString     bgColor            MEMBER _bgColor            NOTIFY bgColorChanged)
	Q_PROPERTY(QString     executionModuleId  MEMBER _executionModuleId  NOTIFY executionModuleIdChanged)
	Q_PROPERTY(QString     cardId             MEMBER _cardId             NOTIFY cardIdChanged)
	Q_PROPERTY(QStringList workModes          READ   workModes           NOTIFY workModesChanged)
	Q_PROPERTY(bool        hasParameters      MEMBER _hasParameters      NOTIFY hasParametersChanged)

signals:
	void qmlPathChanged();
	void nameChanged();
	void bgColorChanged();
	void executionModuleIdChanged();
	void cardIdChanged();
	void workModesChanged();
	void hasParametersChanged();

	void showModuleParameters(const QString& moduleId, const QString& moduleName);

public:
	explicit BaseCard(QObject* parent, const QString& qmlPath);

	Q_INVOKABLE QStringList workModes() const;
	Q_INVOKABLE void showParameters();

protected:
	virtual void setCard(const ai::kernel::ExecutionModule& em, const ai::kernel::ExecutionModule::Card& card);
	int workModeModelIndex(const QString& wmId);
	QString workModeId(int modelIndex);

private:
	void setName(const QString& name);
	void setBgColor(const ai::kernel::ExecutionModule::CardBgColor& bgColor);
	void setExecutionModuleId(const QString& executionModuleId);
	void setCardId(const QString& cardId);
	void setWorkModes(const ::google::protobuf::RepeatedPtrField<ai::kernel::ExecutionModule_WorkMode>& workModes);
	void setHasParameters(bool has);

private:
	QString                  _moduleId;
	QString                  _qmlPath;
	QString                  _name;
	QString                  _bgColor;
	QString                  _executionModuleId;
	QString                  _cardId;
	QStringList              _workModes;
	QHash<QString, QString>  _workModesHash;
	bool                     _hasParameters;
};

} // namespace main
} // namespace ai

#endif // AI_MAIN_BASECARD_H
