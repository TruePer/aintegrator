////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GroupName.h"

using namespace ai::main;

GroupName::GroupName(QObject* parent)
    : Parameter(parent)
    , _type(Group)
{
}

void GroupName::update(ai::proto::Parameter* /*parameter*/)
{
	// do nothink
}

void GroupName::setName(const QString& name)
{
	if(_name != name) {
		_name = name;
		emit changed();
	}
}
