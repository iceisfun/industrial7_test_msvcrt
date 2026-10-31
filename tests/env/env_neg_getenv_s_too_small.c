/* TC
 * id:    env_neg_getenv_s_too_small
 * area:  env
 * kind:  negative
 * procs: getenv_s
 * funcs: _putenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs getenv_s (ERANGE when the buffer is too small; *pReturnValue = required size)
 * note:  Whether the invalid-parameter handler runs for ERANGE is not asserted (see env_neg_probe_getenv_s_erange_handler).
 *        A handler is installed only so that a non-returning default cannot kill the test.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(int, getenv_s, (size_t *, char *, size_t, const char *));

TC_CASE(erange)
{
    size_t n = 99; char b[4] = "abc"; int r;
    if (!tc_p_getenv_s) TC_SKIP("getenv_s not exported");
    tc_iph_install();
    _putenv("TCV_GSR=hello");
    r = tc_p_getenv_s(&n, b, 4, "TCV_GSR");
    CHECK_EQ_I(r, ERANGE);                 /* returns_ERANGE */
    CHECK_EQ_U(n, 6);                      /* return_value_param_equals_required_size */
    _putenv("TCV_GSR=");
}
TC_CASE(short_by_one)
{
    size_t n = 99; char b[8]; int r;
    if (!tc_p_getenv_s) TC_SKIP("getenv_s not exported");
    tc_iph_install();
    _putenv("TCV_GSR=hello");
    r = tc_p_getenv_s(&n, b, 5, "TCV_GSR");   /* needs 6 */
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_U(n, 6);
    n = 99;
    r = tc_p_getenv_s(&n, b, 6, "TCV_GSR");   /* exactly enough */
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(n, 6);
    _putenv("TCV_GSR=");
}

int main(void)
{
    tc_begin(); TC_LOAD_PROC(getenv_s);
    TC_RUN(erange); TC_RUN(short_by_one);
    return tc_end();
}
