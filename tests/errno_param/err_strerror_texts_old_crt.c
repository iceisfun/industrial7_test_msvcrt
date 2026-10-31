/* TC
 * id:    err_strerror_texts_old_crt
 * area:  errno_param
 * funcs: strerror
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  observed
 * ref:   MS docs strerror
 * note:  Split from the UCRT test so a version difference shows. Edit crt after first Windows run.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static volatile int v_e;
static const char *msg(int e) { v_e = e; return strerror(v_e); }

TC_CASE(zero)   { CHECK_STR(msg(0), "No error"); }
TC_CASE(enoent) { CHECK_STR(msg(ENOENT), "No such file or directory"); }
TC_CASE(einval) { CHECK_STR(msg(EINVAL), "Invalid argument"); }
TC_CASE(eacces) { CHECK_STR(msg(EACCES), "Permission denied"); }
TC_CASE(ebadf)  { CHECK_STR(msg(EBADF), "Bad file descriptor"); }
TC_CASE(eexist) { CHECK_STR(msg(EEXIST), "File exists"); }

int main(void)
{
    tc_begin();
    TC_RUN(zero); TC_RUN(enoent); TC_RUN(einval); TC_RUN(eacces); TC_RUN(ebadf); TC_RUN(eexist);
    return tc_end();
}
