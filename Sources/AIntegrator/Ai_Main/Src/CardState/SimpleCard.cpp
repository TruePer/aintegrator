////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "SimpleCard.h"
#include "internal/Ai_ExecutionModule.pb.h"

#include <QDateTime>


using namespace ai::main;

static const int ADDITIONAL_STATUSES_COUNT_MAX = 9;
static const int TEXT_STATUSES_COUNT_MAX       = 3;

SimpleCard::SimpleCard(QObject* parent)
	: BaseCard(parent, "CardState/SimpleCard.qml")
    , _isValid(false)
    , _mainStatus(new StatusIcon(this))
    , _status1(new StatusIcon(this))
    , _status2(new StatusIcon(this))
    , _status3(new StatusIcon(this))
    , _status4(new StatusIcon(this))
    , _status5(new StatusIcon(this))
    , _status6(new StatusIcon(this))
    , _status7(new StatusIcon(this))
    , _status8(new StatusIcon(this))
    , _status9(new StatusIcon(this))
    , _selectedIndex(-1)
{
	_additionalStatuses << _status1 << _status2 << _status3 << _status4 << _status5
	                    << _status6 << _status7 << _status8 << _status9;
	_textStatuses << &_textStatus1 << &_textStatus2 << &_textStatus3;

	_timer.setSingleShot(true);
	connect(&_timer, SIGNAL(timeout()), SIGNAL(isUpdatingChanged()) );
}

void SimpleCard::setCard(const ai::kernel::ExecutionModule& em, const ai::kernel::ExecutionModule::Card& card)
{
	BaseCard::setCard(em, card);

	// Проверим карточку на наличие состояния
	if(!card.has_state() || !card.state().has_simple_state()) {
		if(_prevStateDump.size()) {
			clearStatuses();
		}
		return;
	}

	// Проверим последнее обновление состояния карточки
	quint64 currentTimeStamp = QDateTime::currentDateTime().toTime_t();
	if(currentTimeStamp - card.last_update_state() > em.timing_parameters().status_update_sec()) {
		if(_prevStateDump.size()) {
			clearStatuses();
		}
		return;
	}

	setValidity(true);

	const auto& simpleState = card.state().simple_state();
	const auto& stateDump = simpleState.SerializeAsString();

	if(stateDump == _prevStateDump)
		return;

	_prevStateDump = stateDump;

	if(_timer.isActive()) {
		_timer.start(1); // Если запущен таймер обновления, остановим его (именно так сработает таймаут!!!)
	}

	_mainStatus->update(em, simpleState.main_status());

	for(int i = 0, size = simpleState.additional_statuses_size(); i < ADDITIONAL_STATUSES_COUNT_MAX; ++i) {
		if(i < size) {
			_additionalStatuses[i]->update(em, simpleState.additional_statuses(i));
		}
		else {
			_additionalStatuses[i]->setValid(false);
		}
	}

	for(int i = 0, size = simpleState.text_statuses_size(); i < TEXT_STATUSES_COUNT_MAX; ++i) {
		if(i < size) {
			setTextStatus(_textStatuses[i], QString::fromStdString(simpleState.text_statuses(i)));
		}
		else {
			setTextStatus(_textStatuses[i], QString());
		}
	}

	if(simpleState.has_current_mode_id()) {
		auto selectedWorkModeId = QString::fromStdString(simpleState.current_mode_id());
		setSelectedWorkModeId(selectedWorkModeId);

		int wmId = workModeModelIndex(selectedWorkModeId);
		setSelectedIndex(wmId);
	}

	setWorkModeActionIcon(simpleState.mode_status());
}

void SimpleCard::setTextStatus(QString* status, const QString& text)
{
	if(*status != text) {
		*status = text;
		emit textStatusChanged();
	}
}

void SimpleCard::setValidity(bool valid)
{
	if(_isValid != valid) {
		_isValid = valid;
		emit isValidChanged();
	}
}

void SimpleCard::setWorkModeActionIcon(ai::proto::PostModeStatus wmStatus)
{
	QString iconName;
	switch (wmStatus) {
	case ai::proto::PM_IDLE:
		iconName = "av/play_circle_outline";
		break;

	case ai::proto::PM_STARTING:
	case ai::proto::PM_STOPPING:
		iconName = "action/autorenew";
		break;

	case ai::proto::PM_RUN:
		iconName = "av/pause_circle_outline";
		break;
	}

	if(_workModeActionIcon != iconName) {
		_workModeActionIcon = iconName;
		emit workModeActionIconChanged();
	}
}

void SimpleCard::clearStatuses()
{
	setValidity(false);

	_prevStateDump.clear();
	_mainStatus->setValid(false);

	for(int i = 0; i < ADDITIONAL_STATUSES_COUNT_MAX; ++i) {
		_additionalStatuses[i]->setValid(false);
	}

	for(int i = 0; i < TEXT_STATUSES_COUNT_MAX; ++i) {
		setTextStatus(_textStatuses[i], QString());
	}

	setSelectedIndex(-1);
}

QString SimpleCard::selectedWorkModeId() const
{
	return _selectedWorkModeId;
}

void SimpleCard::setSelectedWorkModeId(const QString& selectedWorkModeId)
{
	if(selectedWorkModeId != _selectedWorkModeId) {
		_selectedWorkModeId = selectedWorkModeId;
		emit selectedWorkModeIdChanged();
	}
}

void SimpleCard::startUpdateTimer()
{
	_timer.start(5000);
	emit isUpdatingChanged();
}

bool SimpleCard::isUpdating()
{
	return _timer.isActive();
}

int SimpleCard::selectedIndex() const
{
	return _selectedIndex;
}

void SimpleCard::setSelectedIndex(int selectedIndex)
{
	if(_selectedIndex != selectedIndex) {
		_selectedIndex = selectedIndex;
		emit selectedIndexChanged();
	}

	auto wmId =  workModeId(selectedIndex);
	setSelectedWorkModeId(wmId);

}
