////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ConnectionSettings.h"



using namespace ai::state;

ConnectionSettings::ConnectionSettings()
	: _port(0)
{
}

const quint16& ConnectionSettings::port() const
{
	return _port;
}

quint16& ConnectionSettings::port()
{
	return _port;
}

void ConnectionSettings::setPort(const quint16& port)
{
	_port = port;
}
