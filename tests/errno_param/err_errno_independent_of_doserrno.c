/* TC
 * id:    err_errno_independent_of_doserrno
 * area:  errno_param
 * funcs: _errno __doserrno
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs errno, _doserrno
 */
#include "tc.h"
#include <errno.h>
#include <stdlib.h>

TC_CASE(separate) {
    errno = EINVAL; _doserrno = 5;
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(_doserrno, 5);
    errno = ERANGE; CHECK_EQ_I(_doserrno, 5);
    _doserrno = 87; CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(_doserrno, 87);
    errno = 0; _doserrno = 0;
}
TC_CASE(ptrs) { CHECK((void *)_errno() != (void *)__doserrno()); }
int main(void) { tc_begin(); TC_RUN(separate); TC_RUN(ptrs); return tc_end(); }
