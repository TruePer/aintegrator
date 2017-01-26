////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef AI_MAIN_PARAMETER_H
#define AI_MAIN_PARAMETER_H

#include "BaseParameter.h"



namespace ai {
namespace main {
class CheckBoxParameter;
class IntervalParameter;
class LineEditParameter;
class SelectListParameter;

class Parameter : public BaseParameter
{
	Q_OBJECT

	Q_PROPERTY(ParameterType type        MEMBER _type        NOTIFY changed)
	Q_PROPERTY(QString       name        MEMBER _name        NOTIFY changed)
	Q_PROPERTY(QString       description MEMBER _description NOTIFY changed)
	Q_PROPERTY(ai::main::CheckBoxParameter*     checkBox   MEMBER _checkBox   NOTIFY changed)
	Q_PROPERTY(ai::main::LineEditParameter*     lineEdit   MEMBER _lineEdit   NOTIFY changed)
	Q_PROPERTY(ai::main::SelectListParameter*   selectList MEMBER _selectList NOTIFY changed)
	Q_PROPERTY(ai::main::IntervalParameter*     interval   MEMBER _interval   NOTIFY changed)

signals:
	void changed();

public:
	explicit Parameter(QObject* parent = nullptr);

public:
	void update(ai::proto::Parameter* parameter);

private:
	ParameterType        _type;
	QString              _name;
	QString              _description;
	CheckBoxParameter*   _checkBox;
	LineEditParameter*   _lineEdit;
	SelectListParameter* _selectList;
	IntervalParameter*   _interval;

};

} // namespace main
} // namespace ai

#endif // AI_MAIN_PARAMETER_H
