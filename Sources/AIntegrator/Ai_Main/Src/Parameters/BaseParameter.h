////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef AI_MAIN_BASEPARAMETER_H
#define AI_MAIN_BASEPARAMETER_H

#include <QObject>



namespace ai {

namespace proto {
class Parameter;
}

namespace main {
class BaseParameter : public QObject
{
	Q_OBJECT
signals:
	void changed();

public:
	enum ParameterType {
		Group = 0,
		CheckBox,
		LineEdit,
		SelectList,
		Interval
	};

	Q_ENUM(ParameterType)

public:
	BaseParameter(QObject* parent = nullptr);

public:
	virtual void update(ai::proto::Parameter* parameter) = 0;

};

} // namespace main
} // namespace ai

#endif // AI_MAIN_BASEPARAMETER_H
