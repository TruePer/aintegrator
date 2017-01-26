////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SelectListParameter.h"
#include "host/Ai_PostParameters.pb.h"



using namespace ai::main;

SelectListParameter::SelectListParameter(QObject* parent)
    : BaseParameter(parent)
    , _type(Unknown)
    , _selectedIndex(-1)
    , _selectList(nullptr)
{
}

void SelectListParameter::update(ai::proto::Parameter* parameter)
{
	if(!parameter->has_select_list())
		return;

	_selectList = parameter->mutable_select_list();

	auto type = static_cast<ParameterType>(_selectList->ui_type());

	if(_type != type) {
		_type = type;
		emit changed();
	}

	QStringList items;

	for(int i = 0, size = _selectList->values_size(); i < size; ++i) {
		items << QString::fromStdString(_selectList->values(i));
	}

	if(_items != items) {
		_items = items;
		emit changed();
	}

	auto selectedIndex = _selectList->current_index();

	if(selectedIndex < 0 || selectedIndex >= items.size()) {
		selectedIndex = -1;
	}

	setSelectedIndex(selectedIndex);
}

void SelectListParameter::setSelectedIndex(int selected)
{
	if(_selectedIndex != selected) {
		_selectedIndex = selected;

		if(_selectList) {
			_selectList->set_current_index(selected);
		}

		emit selectedIndexChanged();
	}
}
