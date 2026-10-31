/* TC
 * id:    err_neg_probe_get_errno_null_doserrno_unchanged
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: _get_errno _get_doserrno _set_doserrno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _get_errno, _get_doserrno, _set_doserrno
 * note:  PROBE: the failing _get_errno(NULL)/_get_doserrno(NULL) calls set errno (EINVAL) but leave _doserrno untouched
 *        (a preset sentinel 0x1234 is still there afterwards). Docs do not state it explicitly.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(errno_t, _get_errno, (int *));
TC_DECL_PROC(errno_t, _get_doserrno, (unsigned long *));
TC_DECL_PROC(errno_t, _set_doserrno, (unsigned long));

TC_CASE(doserrno_unchanged)
{
    unsigned long d = 0; errno_t r;
    CHECK_EQ_I(tc_p__set_doserrno(0x1234), 0);
    tc_iph_count = 0;
    r = tc_p__get_errno(NULL); CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_p__get_doserrno(&d), 0); CHECK_EQ_U(d, 0x1234);
    r = tc_p__get_doserrno(NULL); CHECK_EQ_I(r, EINVAL);
    d = 0; CHECK_EQ_I(tc_p__get_doserrno(&d), 0); CHECK_EQ_U(d, 0x1234);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_errno); TC_LOAD_PROC(_get_doserrno); TC_LOAD_PROC(_set_doserrno);
    if (!tc_p__get_errno || !tc_p__get_doserrno || !tc_p__set_doserrno || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(doserrno_unchanged);
    tc_p__set_doserrno(0); errno = 0;
    return tc_end();
}
