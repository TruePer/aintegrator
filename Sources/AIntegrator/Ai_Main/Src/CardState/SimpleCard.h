////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_MAIN_CARDSTATE_H
#define AI_MAIN_CARDSTATE_H

#include "BaseCard.h"
#include "Icon.h"

#include <QTimer>



namespace ai {
namespace main {

class SimpleCard : public BaseCard
{
	Q_OBJECT
	Q_PROPERTY(bool                  isValid     MEMBER _isValid            NOTIFY isValidChanged)
	Q_PROPERTY(ai::main::StatusIcon* mainStatus  MEMBER _mainStatus         NOTIFY mainStatusChanged)
	Q_PROPERTY(ai::main::StatusIcon* status1     MEMBER _status1            NOTIFY additionalStatusChanged)
	Q_PROPERTY(ai::main::StatusIcon* status2     MEMBER _status2            NOTIFY additionalStatusChanged)
	Q_PROPERTY(ai::main::StatusIcon* status3     MEMBER _status3            NOTIFY additionalStatusChanged)
	Q_PROPERTY(ai::main::StatusIcon* status4     MEMBER _status4            NOTIFY additionalStatusChanged)
	Q_PROPERTY(ai::main::StatusIcon* status5     MEMBER _status5            NOTIFY additionalStatusChanged)
	Q_PROPERTY(ai::main::StatusIcon* status6     MEMBER _status6            NOTIFY additionalStatusChanged)
	Q_PROPERTY(ai::main::StatusIcon* status7     MEMBER _status7            NOTIFY additionalStatusChanged)
	Q_PROPERTY(ai::main::StatusIcon* status8     MEMBER _status8            NOTIFY additionalStatusChanged)
	Q_PROPERTY(ai::main::StatusIcon* status9     MEMBER _status9            NOTIFY additionalStatusChanged)
	Q_PROPERTY(QString               textStatus1 MEMBER _textStatus1        NOTIFY textStatusChanged)
	Q_PROPERTY(QString               textStatus2 MEMBER _textStatus2        NOTIFY textStatusChanged)
	Q_PROPERTY(QString               textStatus3 MEMBER _textStatus3        NOTIFY textStatusChanged)
	Q_PROPERTY(int                 selectedIndex READ selectedIndex         WRITE setSelectedIndex      NOTIFY selectedIndexChanged)
	Q_PROPERTY(QString        selectedWorkModeId READ selectedWorkModeId    WRITE setSelectedWorkModeId NOTIFY selectedWorkModeIdChanged)
	Q_PROPERTY(QString        workModeActionIcon MEMBER _workModeActionIcon NOTIFY workModeActionIconChanged)
	Q_PROPERTY(bool                  isUpdating  READ isUpdating            NOTIFY isUpdatingChanged)

signals:
	void isValidChanged();
	void mainStatusChanged();
	void additionalStatusChanged();
	void textStatusChanged();
	void selectedIndexChanged();
	void selectedWorkModeIdChanged();
	void workModeActionIconChanged();
	void isUpdatingChanged();

public:
	SimpleCard(QObject* parent = nullptr);

public:
	void setCard(const ai::kernel::ExecutionModule& em, const ai::kernel::ExecutionModule::Card& card);

	Q_INVOKABLE int selectedIndex() const;
	Q_INVOKABLE void setSelectedIndex(int selectedIndex);

	Q_INVOKABLE QString selectedWorkModeId() const;
	Q_INVOKABLE void setSelectedWorkModeId(const QString& selectedWorkModeId);

	Q_INVOKABLE void startUpdateTimer();
	Q_INVOKABLE bool isUpdating();

private:
	void setTextStatus(QString* status, const QString& text);
	void setValidity(bool valid);
	void setWorkModeActionIcon(ai::proto::PostModeStatus wmStatus);
	void clearStatuses();

private:
	bool               _isValid;

	StatusIcon*        _mainStatus;

	StatusIcon*        _status1;
	StatusIcon*        _status2;
	StatusIcon*        _status3;
	StatusIcon*        _status4;
	StatusIcon*        _status5;
	StatusIcon*        _status6;
	StatusIcon*        _status7;
	StatusIcon*        _status8;
	StatusIcon*        _status9;
	QList<StatusIcon*> _additionalStatuses;

	QString            _textStatus1;
	QString            _textStatus2;
	QString            _textStatus3;
	QList<QString*>    _textStatuses;

	int                _selectedIndex;
	QString            _selectedWorkModeId;
	QString            _workModeActionIcon;

	QTimer             _timer;
	std::string        _prevStateDump;
};

} // namespace main
} // namespace ai

//Q_DECLARE_METATYPE(ai::main::SimpleCard)

#endif // AI_MAIN_CARDSTATE_H
