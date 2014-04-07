#ifndef RUNE_GLOBAL_H
#define RUNE_GLOBAL_H

#include <QDebug>
#include <QtCore/qglobal.h>

#define RUNE_VERSION 0.1

#if defined(RUNE_LIBRARY)
#  define RUNESHARED_EXPORT Q_DECL_EXPORT
#else
#  define RUNESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RUNE_GLOBAL_H
