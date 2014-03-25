#ifndef RUNE_GLOBAL_H
#define RUNE_GLOBAL_H

#include <QDebug>

#if defined(RUNE_LIBRARY)
#  define RUNESHARED_EXPORT Q_DECL_EXPORT
#else
#  define RUNESHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QtCore/qglobal.h>

#endif // RUNE_GLOBAL_H
