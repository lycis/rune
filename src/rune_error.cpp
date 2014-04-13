#include "rune/rune_error.h"

quint64 rune::lastErrcd;

quint64 rune::lastError()
{
    quint64 r = lastErrcd;
    lastErrcd = RUNE_ERR_NO;
    return r;
}

void rune::setError(quint64 errcd)
{
    lastErrcd = errcd;
}
