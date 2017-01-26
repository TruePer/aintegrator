////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef AI_MAIN_LINEEDITPARAMETER_H
#define AI_MAIN_LINEEDITPARAMETER_H

#include "BaseParameter.h"
#include <QValidator>



namespace ai {

namespace proto {
class LineEditParameter;
}

namespace main {

class LineEditParameter : public BaseParameter
{
	Q_OBJECT
	Q_PROPERTY(QString       value        MEMBER _value        NOTIFY changed)
	Q_PROPERTY(QValidator*   validator    MEMBER _validator    NOTIFY changed)

signals:
	void changed();

public:
	explicit LineEditParameter(QObject* parent = nullptr);

public:
	void update(ai::proto::Parameter* parameter);
	Q_INVOKABLE void setValue(const QString& value);

private:
	enum ParameterType {
		Unknown = 0,
		SignedInt,
		UnsignedInt,
		SignedDouble,
		StringValue
	};

private:
	ParameterType             _type;
	QString                   _value;
	quint32                   _maxSymbols;
	QValidator*               _validator;
	proto::LineEditParameter* _lineEdit;
};

} // namespace main
} // namespace ai

#endif // AI_MAIN_LINEEDITPARAMETER_H
