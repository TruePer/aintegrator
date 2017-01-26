////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef AI_MAIN_CHECKBOXPARAMETER_H
#define AI_MAIN_CHECKBOXPARAMETER_H

#include "BaseParameter.h"



namespace ai {

namespace proto {
class Parameter;
class CheckBoxParameter;
}

namespace main {

class CheckBoxParameter : public BaseParameter
{
	Q_OBJECT
	Q_PROPERTY(bool checked MEMBER _checked NOTIFY changed)

signals:
	void changed();

public:
	explicit CheckBoxParameter(QObject* parent = nullptr);

public:
	void update(ai::proto::Parameter* parameter);
	Q_INVOKABLE void setChecked(bool checked);

private:
	bool                          _checked;
	proto::CheckBoxParameter*     _checkBox;

};

} // namespace main
} // namespace ai

#endif // AI_MAIN_CHECKBOXPARAMETER_H
