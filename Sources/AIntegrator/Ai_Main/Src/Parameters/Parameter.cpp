////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CheckBoxParameter.h"
#include "IntervalParameter.h"
#include "LineEditParameter.h"
#include "Parameter.h"
#include "SelectListParameter.h"

#include "host/Ai_PostParameters.pb.h"



using namespace ai::main;

Parameter::Parameter(QObject* parent)
    : BaseParameter(parent)
    , _type(Group)
    , _checkBox(new CheckBoxParameter(this))
    , _lineEdit(new LineEditParameter(this))
    , _selectList(new SelectListParameter(this))
    , _interval(new IntervalParameter(this))
{
}

void Parameter::update(ai::proto::Parameter* parameter)
{
	auto type = static_cast<ParameterType>(parameter->type());

	if(_type != type) {
		_type = type;
		emit changed();
	}

	auto name = QString::fromStdString(parameter->name());

	if(_name != name) {
		_name = name;
		emit changed();
	}

	auto description = QString::fromStdString(parameter->description());

	if(_description != description) {
		_description = description;
		emit changed();
	}

	_checkBox->update(parameter);
	_lineEdit->update(parameter);
	_selectList->update(parameter);
	_interval->update(parameter);
}
