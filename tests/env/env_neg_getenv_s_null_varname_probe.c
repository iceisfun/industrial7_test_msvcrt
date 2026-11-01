/* TC
 * id:    env_neg_getenv_s_null_varname_probe
 * area:  env
 * funcs:
 * procs: getenv_s
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs getenv_s
 * note:  PROBE: getenv_s with varname == NULL invokes the invalid-parameter handler and returns EINVAL (not listed
 *        explicitly in the docs error table).
 *        Win10 22H2 19045 (observed): getenv_s with varname == NULL is NOT rejected: returns 0, *pReturnValue = 0, no handler call. (MS docs list EINVAL;
 *        Windows deviates from the docs table.)
 *        [unverified] on other CRTs: only ucrtbase 19045 was observed; the expectation is gated with TC_AT_LEAST(140) and the original expectation is kept for older CRTs.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(errno_t, getenv_s, (size_t *, char *, size_t, const char *));

TC_CASE(null_varname_einval)
{
    char buf[8]; size_t n = 5; errno_t r;
    if (!tc_p_getenv_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0;
    r = tc_p_getenv_s(&n, buf, sizeof buf, NULL);

#if TC_AT_LEAST(140)
    CHECK_EQ_I(r, 0);               /* observed: not validated */
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_U(n, 0);
#else
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
#endif

}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(getenv_s);
    TC_RUN(null_varname_einval);
    return tc_end();
}
