#ifndef AI_KERNEL_EXPORT_H
#define AI_KERNEL_EXPORT_H

#include <QtCore/qglobal.h>

#ifndef AI_KERNEL_STATIC_BUILD
#  ifdef AI_KERNEL_BUILD
#    define Kernel_EXPORT Q_DECL_EXPORT
#  else
#    define Kernel_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Kernel_EXPORT
#endif

#endif // AI_KERNEL_EXPORT_H
