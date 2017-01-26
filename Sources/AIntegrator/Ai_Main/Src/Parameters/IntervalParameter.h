////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef AI_MAIN_INTERVALPARAMETER_H
#define AI_MAIN_INTERVALPARAMETER_H

#include "BaseParameter.h"



namespace ai {

namespace proto {
class ValueFromIntervalParameter;
}

namespace main {

class IntervalParameter : public BaseParameter
{
	Q_OBJECT
	Q_PROPERTY(ParameterType type          MEMBER _type         NOTIFY changed)
	Q_PROPERTY(qint32        minValue      MEMBER _minValue     NOTIFY changed)
	Q_PROPERTY(qint32        maxValue      MEMBER _maxValue     NOTIFY changed)
	Q_PROPERTY(qint32        value         MEMBER _value        NOTIFY changed)
	Q_PROPERTY(quint32       step          MEMBER _step         NOTIFY changed)

signals:
	void changed();

public:
	enum ParameterType {
		Unknown = 0,
		Slider,
		SpinBox
	};
	Q_ENUM(ParameterType)

public:
	explicit IntervalParameter(QObject* parent = nullptr);

public:
	void update(ai::proto::Parameter* parameter);
	Q_INVOKABLE void setValue(const qint32& value);

private:
	ParameterType               _type;
	qint32                      _value;
	qint32                      _minValue;
	qint32                      _maxValue;
	quint32                     _step;
	proto::ValueFromIntervalParameter*   _intervalParameter;
};

} // namespace main
} // namespace ai

#endif // AI_MAIN_INTERVALPARAMETER_H
