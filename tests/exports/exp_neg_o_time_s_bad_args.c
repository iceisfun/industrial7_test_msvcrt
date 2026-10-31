/* TC
 * id:    exp_neg_o_time_s_bad_args
 * area:  exports
 * funcs:
 * procs: _o__gmtime64_s _o__localtime64_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _gmtime64_s, _localtime64_s (EINVAL on NULL / beyond 3000-12-31; fields set to -1)
 * note:  PROBE: _o_ time aliases reject NULL args and out-of-range times.
 *        The _o_ aliases are undocumented; expectations mirror the documented behaviour of the underlying function.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

typedef int (__cdecl *tm_s_t)(struct tm *, const long long *);
static tm_s_t p_gm, p_loc;
static const long long tbig = 0x100000000000LL;

static int all_minus1(const struct tm *t)
{ return t->tm_sec == -1 && t->tm_min == -1 && t->tm_hour == -1 && t->tm_mday == -1 && t->tm_mon == -1 &&
         t->tm_year == -1 && t->tm_wday == -1 && t->tm_yday == -1 && t->tm_isdst == -1; }

static void null_checks(tm_s_t f)
{
    struct tm t; long long v = 0; int r; volatile long long *pv = &v; const long long *nullt = NULL;
    if (!tc_iph_install()) return;
    tc_iph_count = 0;
    r = f(NULL, (const long long *)pv);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    memset(&t, 0, sizeof t);
    r = f(&t, nullt);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 2); CHECK(all_minus1(&t));
}
static void range_check(tm_s_t f)
{
    struct tm t; long long v = tbig; int r;
    memset(&t, 0, sizeof t);
    r = f(&t, &v);
    CHECK_EQ_I(r, EINVAL); CHECK(all_minus1(&t));
}
TC_CASE(gm_null) { if (!p_gm) TC_SKIP("not exported"); null_checks(p_gm); }
TC_CASE(gm_range) { if (!p_gm) TC_SKIP("not exported"); if (!tc_iph_install()) TC_SKIP("no handler api"); range_check(p_gm); }
TC_CASE(loc_null) { if (!p_loc) TC_SKIP("not exported"); null_checks(p_loc); }
TC_CASE(loc_range) { if (!p_loc) TC_SKIP("not exported"); if (!tc_iph_install()) TC_SKIP("no handler api"); range_check(p_loc); }
int main(void)
{
    tc_begin();
    p_gm = (tm_s_t)tc_crt_proc("_o__gmtime64_s"); p_loc = (tm_s_t)tc_crt_proc("_o__localtime64_s");
    TC_RUN(gm_null); TC_RUN(gm_range); TC_RUN(loc_null); TC_RUN(loc_range);
    return tc_end();
}
