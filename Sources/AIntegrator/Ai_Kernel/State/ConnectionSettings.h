////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CONNECTIONSETTINGS_H
#define CONNECTIONSETTINGS_H

#include "Common/ChangeableReference.h"



namespace ai {
namespace state {

/**
 * Данные о сетевом соединении
 */
class Kernel_EXPORT ConnectionSettings
{
public:
	typedef common::ChangeableReference<ConnectionSettings> ChRef;

public:
	ConnectionSettings();

	const quint16& port() const;
	quint16& port();
	void setPort(const quint16& port);

private:
	quint16 _port;                                         // Порт
};

}
}

#endif // CONNECTIONSETTINGS_H
