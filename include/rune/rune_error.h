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

namespace rune {

/**
  * @brief gives the last thrown error code and resets it
  */
RUNESHARED_EXPORT quint64 lastError();


extern quint64 lastErrcd;
void setError(quint64 errcd);

}

#endif // ERROR_H
