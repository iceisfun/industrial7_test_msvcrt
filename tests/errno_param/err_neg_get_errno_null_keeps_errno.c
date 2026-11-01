/* TC
 * id:    err_neg_get_errno_null_keeps_errno
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: _get_errno _get_doserrno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _get_errno, _get_doserrno: NULL pointer -> invalid parameter handler, return EINVAL (errno set to EINVAL)
 * note:  OBSERVED on Win10 22H2 (19045): _get_errno(NULL) and _get_doserrno(NULL) return EINVAL and call the handler once but
 *        leave errno unchanged (ERANGE preset), as the test id says. The earlier expectation errno==EINVAL contradicted the id.
 *        Stable.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(errno_t, _get_errno, (int *));
TC_DECL_PROC(errno_t, _get_doserrno, (unsigned long *));

TC_CASE(get_errno_null)
{
    errno_t r; tc_iph_count = 0; errno = ERANGE;
    r = tc_p__get_errno(NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, ERANGE);   /* observed: errno left unchanged */ CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(get_doserrno_null)
{
    errno_t r; tc_iph_count = 0; errno = ERANGE;
    r = tc_p__get_doserrno(NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, ERANGE);   /* observed: errno left unchanged */ CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_errno); TC_LOAD_PROC(_get_doserrno);
    if (!tc_p__get_errno || !tc_p__get_doserrno || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(get_errno_null); TC_RUN(get_doserrno_null);
    errno = 0;
    return tc_end();
}
