/* TC
 * id:    err_dosmaperr_basic_probe
 * area:  errno_param
 * funcs: _dosmaperr
 * crt:   msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   CRT internals (dosmap table); undocumented
 * note:  PROBE: _dosmaperr maps a Win32 error to errno and stores it in _doserrno. 5->EACCES, 2->ENOENT, 87->EINVAL. Not exported by ucrtbase.
 */
#include "tc.h"
#include <errno.h>
#include <stdlib.h>

extern void __cdecl _dosmaperr(unsigned long);

TC_CASE(access_denied) { errno = 0; _dosmaperr(5); CHECK_EQ_I(errno, EACCES); CHECK_EQ_I(_doserrno, 5); }
TC_CASE(file_not_found) { errno = 0; _dosmaperr(2); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(_doserrno, 2); }
TC_CASE(invalid_parameter) { errno = 0; _dosmaperr(87); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(_doserrno, 87); }

int main(void)
{
    tc_begin();
    TC_RUN(access_denied); TC_RUN(file_not_found); TC_RUN(invalid_parameter);
    errno = 0;
    return tc_end();
}
