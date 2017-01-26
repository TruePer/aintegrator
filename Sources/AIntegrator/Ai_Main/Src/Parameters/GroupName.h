////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef AI_MAIN_GROUPNAME_H
#define AI_MAIN_GROUPNAME_H

#include "Parameter.h"



namespace ai {
namespace main {

class GroupName : public Parameter
{
	Q_OBJECT
	Q_PROPERTY(ParameterType       type        MEMBER _type        NOTIFY changed)
	Q_PROPERTY(QString                        name        MEMBER _name        NOTIFY changed)

signals:
	void changed();

public:
	GroupName(QObject* parent = nullptr);

public:
	void update(ai::proto::Parameter* parameter);
	void setName(const QString& name);


private:
	ParameterType _type;
	QString       _name;
};

} // namespace main
} // namespace ai

#endif // AI_MAIN_GROUPNAME_H
