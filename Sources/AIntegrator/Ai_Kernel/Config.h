////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_KERNEL_CONFIG_H
#define AI_KERNEL_CONFIG_H

#ifdef __unix__
#  ifdef QT_NO_DEBUG
#    define CONFIGURATION_DIRECTORY "/etc/default/AIntegrator/Config"
#  else
#    define CONFIGURATION_DIRECTORY "/home/user/.AIntegrator/Config"
#  endif
#else
#  define CONFIGURATION_DIRECTORY "c:\\PostWin Software\\AIntegrator\\Config"
#endif

#define AI_CONFIGURATION_FILE "AIntegrator.xml"

#endif // AI_KERNEL_CONFIG_H
