/* TC
 * id:    dat_neg_get_tz_null_old_msvcr90_80
 * area:  data_exports
 * funcs: 
 * procs: _get_daylight _get_timezone _get_dstbias _get_tzname
 * crt:   msvcr90 msvcr80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_daylight, _get_timezone, _get_dstbias, _get_tzname (VS2005+): EINVAL on bad parameter
 */
#include "tc.h"
#include <errno.h>
#include <stddef.h>
TC_DECL_PROC(int, _get_daylight, (int *));
TC_DECL_PROC(int, _get_timezone, (long *));
TC_DECL_PROC(int, _get_dstbias, (long *));
TC_DECL_PROC(int, _get_tzname, (size_t *, char *, size_t, int));

TC_CASE(nulls)
{
    int c, r; void *volatile np = NULL;
    if (!tc_p__get_daylight || !tc_p__get_timezone || !tc_p__get_dstbias) TC_SKIP("no proc");
    c = tc_iph_count;
    r = tc_p__get_daylight((int *)np); CHECK_EQ_I(r, EINVAL);
    r = tc_p__get_timezone((long *)np); CHECK_EQ_I(r, EINVAL);
    r = tc_p__get_dstbias((long *)np); CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, c + 3);
}
TC_CASE(tzname_bad)
{
    size_t n = 0; char b[16]; int r; char *volatile nb = NULL;
    if (!tc_p__get_tzname) TC_SKIP("no proc");
    r = tc_p__get_tzname(&n, nb, 16, 0); CHECK_EQ_I(r, EINVAL);
    r = tc_p__get_tzname(&n, b, sizeof b, 5); CHECK_EQ_I(r, EINVAL);
}
TC_CASE(valid_after_failures)
{
    int d = -1; long t = -1, s = -1; size_t n = 0; char b[64];  /* default tzname is the Windows zone name, up to 63 chars */
    if (!tc_p__get_daylight || !tc_p__get_timezone || !tc_p__get_dstbias || !tc_p__get_tzname) TC_SKIP("no proc");
    CHECK_EQ_I(tc_p__get_daylight(&d), 0); CHECK_EQ_I(tc_p__get_timezone(&t), 0);
    CHECK_EQ_I(tc_p__get_dstbias(&s), 0); CHECK_EQ_I(tc_p__get_tzname(&n, b, sizeof b, 0), 0);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_get_daylight); TC_LOAD_PROC(_get_timezone); TC_LOAD_PROC(_get_dstbias); TC_LOAD_PROC(_get_tzname);
    TC_RUN(nulls); TC_RUN(tzname_bad); TC_RUN(valid_after_failures);
    return tc_end();
}
