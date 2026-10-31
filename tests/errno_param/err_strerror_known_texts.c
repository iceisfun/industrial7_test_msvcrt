/* TC
 * id:    err_strerror_known_texts
 * area:  errno_param
 * funcs: strerror
 * crt:   >=140
 * tier:  p0
 * conf:  observed
 * ref:   MS docs strerror, errno constants
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static volatile int v_e;
static const char *msg(int e) { v_e = e; return strerror(v_e); }

TC_CASE(zero)    { CHECK_STR(msg(0), "No error"); }
TC_CASE(enoent)  { CHECK_STR(msg(ENOENT), "No such file or directory"); }
TC_CASE(einval)  { CHECK_STR(msg(EINVAL), "Invalid argument"); }
TC_CASE(eacces)  { CHECK_STR(msg(EACCES), "Permission denied"); }
TC_CASE(ebadf)   { CHECK_STR(msg(EBADF), "Bad file descriptor"); }
TC_CASE(eexist)  { CHECK_STR(msg(EEXIST), "File exists"); }
TC_CASE(enomem)  { CHECK_STR(msg(ENOMEM), "Not enough space"); }
TC_CASE(eperm_enospc) { CHECK_STR(msg(EPERM), "Operation not permitted"); CHECK_STR(msg(ENOSPC), "No space left on device"); }

int main(void)
{
    tc_begin();
    TC_RUN(zero); TC_RUN(enoent); TC_RUN(einval); TC_RUN(eacces); TC_RUN(ebadf);
    TC_RUN(eexist); TC_RUN(enomem); TC_RUN(eperm_enospc);
    return tc_end();
}
