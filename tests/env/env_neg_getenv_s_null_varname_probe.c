/* TC
 * id:    env_neg_getenv_s_null_varname_probe
 * area:  env
 * funcs:
 * procs: getenv_s
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs getenv_s
 * note:  PROBE: getenv_s with varname == NULL invokes the invalid-parameter handler and returns EINVAL (not listed
 *        explicitly in the docs error table).
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
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(getenv_s);
    TC_RUN(null_varname_einval);
    return tc_end();
}
