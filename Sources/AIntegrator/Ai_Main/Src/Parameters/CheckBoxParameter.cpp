////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CheckBoxParameter.h"
#include "host/Ai_PostParameters.pb.h"



using namespace ai::main;

CheckBoxParameter::CheckBoxParameter(QObject* parent)
    : BaseParameter(parent)
    , _checked(false)
    , _checkBox(nullptr)
{
}

void CheckBoxParameter::update(ai::proto::Parameter* parameter)
{
	if(!parameter->has_check_box())
		return;

	bool checked = parameter->check_box().checked();
	_checkBox = parameter->mutable_check_box();
	setChecked(checked);
}

/**
 * Установка из UI
 * @param checked
 */
void CheckBoxParameter::setChecked(bool checked)
{
	if(_checkBox) {
		_checkBox->set_checked(checked);
	}

	if(_checked != checked) {
		_checked = checked;
		emit changed();
	}
}
