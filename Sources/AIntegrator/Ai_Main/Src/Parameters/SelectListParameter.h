////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef AI_MAIN_SELECTLISTPARAMETER_H
#define AI_MAIN_SELECTLISTPARAMETER_H

#include "BaseParameter.h"



namespace ai {

namespace proto {
class SelectListParameter;
}

namespace main {

class SelectListParameter : public BaseParameter
{
	Q_OBJECT
	Q_PROPERTY(ParameterType type          MEMBER _type         NOTIFY changed)
	Q_PROPERTY(QStringList   items         MEMBER _items        NOTIFY changed)
	Q_PROPERTY(int           selectedIndex MEMBER _selectedIndex NOTIFY selectedIndexChanged)

signals:
	void changed();
	void selectedIndexChanged();

public:
	enum ParameterType {
		Unknown = 0,
		ComboBox,
		RadioButtons
	};
	Q_ENUM(ParameterType)

public:
	explicit SelectListParameter(QObject* parent = nullptr);

public:
	void update(ai::proto::Parameter* parameter);
	Q_INVOKABLE void setSelectedIndex(int selected);

private:
	ParameterType               _type;
	QStringList                 _items;
	int                         _selectedIndex;
	proto::SelectListParameter* _selectList;
};

} // namespace main
} // namespace ai

#endif // AI_MAIN_SELECTLISTPARAMETER_H
