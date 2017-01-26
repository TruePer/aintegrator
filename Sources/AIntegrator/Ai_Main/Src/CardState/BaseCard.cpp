////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "BaseCard.h"

using namespace ai::main;

BaseCard::BaseCard(QObject* parent, const QString& qmlPath)
    : QObject(parent)
    , _qmlPath(qmlPath)
{
}

QStringList BaseCard::workModes() const
{
	return _workModes;
}

void BaseCard::showParameters()
{
	emit showModuleParameters(_moduleId, _name);
}

void BaseCard::setCard(const ai::kernel::ExecutionModule& em, const ai::kernel::ExecutionModule::Card& card)
{
	_moduleId = QString::fromStdString(em.id());

	setName(QString::fromStdString(em.name()));

	if(em.is_connected()) {
		setBgColor(card.bg_color());
	} else {
		setBgColor(ai::kernel::ExecutionModule_CardBgColor_BG_GREY);
	}

	setExecutionModuleId(QString::fromStdString(em.id()));
	setCardId(QString::fromStdString(card.id()));
	setWorkModes(em.work_modes());
	setHasParameters(em.has_parameters() && em.parameters().groups_size());
}

int BaseCard::workModeModelIndex(const QString& wmId)
{
	if(_workModesHash.contains(wmId)) {
		auto wm = _workModesHash[wmId];
		return workModes().indexOf(wm);
	}

	return -1;
}

QString BaseCard::workModeId(int modelIndex)
{
	if(modelIndex > -1 && modelIndex < _workModes.count()) {
		auto wm = _workModes[modelIndex];
		for(auto it =_workModesHash.begin(), end = _workModesHash.end(); it != end; ++it) {
			if(it.value() == wm) {
				return it.key();
			}
		}
	}

	return QString();
}

void BaseCard::setName(const QString& name)
{
	if(name != _name) {
		_name = name;
		emit nameChanged();
	}
}

void BaseCard::setBgColor(const ai::kernel::ExecutionModule::CardBgColor& bgColor)
{
	QString color = "white";

	switch(bgColor)  {
	case kernel::ExecutionModule_CardBgColor_BG_UNKNOWN:
		color =  "white";
		break;
	case kernel::ExecutionModule_CardBgColor_BG_RED:
		color =  "red";
		break;
	case kernel::ExecutionModule_CardBgColor_BG_GREEN:
		color =  "green";
		break;
	case kernel::ExecutionModule_CardBgColor_BG_BLUE:
		color =  "blue";
		break;
	case kernel::ExecutionModule_CardBgColor_BG_LIGHTBLUE:
		color =  "lightBlue";
		break;
	case kernel::ExecutionModule_CardBgColor_BG_YELLOW:
		color =  "amber";
		break;
	case kernel::ExecutionModule_CardBgColor_BG_PURPLE:
		color =  "purple";
		break;
	case kernel::ExecutionModule_CardBgColor_BG_CYAN:
		color =  "cyan";
		break;
	case kernel::ExecutionModule_CardBgColor_BG_ORANGE:
		color =  "orange";
		break;
	case kernel::ExecutionModule_CardBgColor_BG_TEAL:
		color =  "teal";
		break;
	case kernel::ExecutionModule_CardBgColor_BG_INDIGO:
		color =  "indigo";
		break;
	case kernel::ExecutionModule_CardBgColor_BG_GREY:
		color =  "grey";
		break;
	case kernel::ExecutionModule_CardBgColor_BG_BLUEGREY:
		color =  "blueGrey";
		break;
	}

	if(_bgColor != color) {
		_bgColor = color;
		emit bgColorChanged();
	}
}

void BaseCard::setExecutionModuleId(const QString& executionModuleId)
{
	if(_executionModuleId != executionModuleId) {
		_executionModuleId = executionModuleId;
		emit executionModuleIdChanged();
	}
}

void BaseCard::setCardId(const QString& cardId)
{
	if(_cardId != cardId) {
		_cardId = cardId;
		emit cardIdChanged();
	}
}

void BaseCard::setWorkModes(const::google::protobuf::RepeatedPtrField<ai::kernel::ExecutionModule_WorkMode>& workModes)
{
	_workModesHash.clear();

	QStringList wmList;

	for(int i = 0, size = workModes.size(); i < size; ++i) {
		_workModesHash.insert(QString::fromStdString(workModes.Get(i).id()), QString::fromStdString(workModes.Get(i).name()));
		wmList << QString::fromStdString(workModes.Get(i).name());
	}

	if(_workModes != wmList) {
		_workModes = wmList;
		emit workModesChanged();
	}
}

void BaseCard::setHasParameters(bool has)
{
	if(_hasParameters != has) {
		_hasParameters = has;
		emit hasParametersChanged();
	}
}
