#ifndef RUNE_GLOBAL_H
#define RUNE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RUNE_LIBRARY)
#  define RUNESHARED_EXPORT Q_DECL_EXPORT
#else
#  define RUNESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RUNE_GLOBAL_H
