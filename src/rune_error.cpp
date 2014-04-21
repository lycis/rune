#include "rune/rune_error.h"

quint64 rune::lastErrcd;
QString rune::lastErrStr;

quint64 rune::lastError()
{
    quint64 r = lastErrcd;
    lastErrcd = RUNE_ERR_NO;
    return r;
}

QString rune::lastErrorStr()
{
    return lastErrStr;
}

void rune::setError(quint64 errcd, QString errstr)
{
    lastErrcd = errcd;
    lastErrStr = errstr;
}
