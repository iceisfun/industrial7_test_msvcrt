/* TC
 * id:    err_errno_ptr_basic
 * area:  errno_param
 * funcs: _errno
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs errno, _doserrno, _sys_errlist, and _sys_nerr (errno is a modifiable lvalue)
 */
#include "tc.h"
#include <errno.h>

TC_CASE(nonnull) { CHECK(_errno() != NULL); }
TC_CASE(zero) { errno = 0; CHECK_EQ_I(errno, 0); CHECK_EQ_I(*_errno(), 0); }
TC_CASE(values) {
    volatile int v[] = { EINVAL, ERANGE, 12345 }; int i;
    for (i = 0; i < 3; i++) { errno = v[i]; CHECK_EQ_I(errno, v[i]); }
    errno = 0;
}
TC_CASE(through_ptr) {
    int *p = _errno(); errno = EDOM; CHECK_EQ_I(*p, EDOM);
    *p = ERANGE; CHECK_EQ_I(errno, ERANGE); errno = 0;
}
int main(void) { tc_begin(); TC_RUN(nonnull); TC_RUN(zero); TC_RUN(values); TC_RUN(through_ptr); return tc_end(); }
