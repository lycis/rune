// contains error handling functions

#ifndef ERROR_H
#define ERROR_H

#include "rune_global.h"

// error codes
#define RUNE_ERR_NO                0x0
#define RUNE_ERR_MAP_LOADED_BEFORE 0x1
#define RUNE_ERR_ACCESS_DENIED     0x2
#define RUNE_ERR_NOT_EXISTS        0x3
#define RUNE_ERR_FILE_FORMAT       0x4
#define RUNE_ERR_SCRIPT            0x5

namespace rune {

/**
  * @brief gives the last thrown error code and resets it
  */
RUNESHARED_EXPORT quint64 lastError();
RUNESHARED_EXPORT QString lastErrorStr();


extern quint64 lastErrcd;
extern QString lastErrStr;
void setError(quint64 errcd, QString errstr = QString());

}

#endif // ERROR_H
