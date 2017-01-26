////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "IntervalParameter.h"
#include "host/Ai_PostParameters.pb.h"



using namespace ai::main;

IntervalParameter::IntervalParameter(QObject* parent)
    : BaseParameter(parent)
    , _type(Unknown)
    , _value(0)
    , _minValue(0)
    , _maxValue(100)
    , _step(1)
{
}

void IntervalParameter::update(ai::proto::Parameter* parameter)
{
	if(!parameter->has_value_from_interval())
		return;

	_intervalParameter = parameter->mutable_value_from_interval();

	auto type = static_cast<ParameterType>(_intervalParameter->ui_type());

	if(_type != type) {
		_type = type;
		emit changed();
	}

	auto min = _intervalParameter->min_value();

	if(_minValue != min) {
		_minValue = min;
		emit changed();
	}

	auto max = _intervalParameter->max_value();

	if(_maxValue != max) {
		_maxValue = max;
		emit changed();
	}

	auto value = _intervalParameter->current_value();
	setValue(value);

	auto step = _intervalParameter->step();

	if(_step != step) {
		_step = step;
		emit changed();
	}
}

void IntervalParameter::setValue(const qint32& value)
{
	if(_intervalParameter) {
		_intervalParameter->set_current_value(value);
	}

	if(_value != value) {
		_value = value;
		emit changed();
	}
}
