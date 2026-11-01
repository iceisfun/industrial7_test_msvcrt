/* TC
 * id:    exp_neg_o_time_s_negative_probe
 * area:  exports
 * funcs:
 * procs: _o__gmtime64_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _gmtime64_s (range starts at 1970-01-01)
 * note:  PROBE: _o__gmtime64_s(-1): docs give a 1970 lower bound; whether UCRT 10 still rejects pre-1970 values is what is measured (expected EINVAL, fields -1).
 *        The _o_ aliases are undocumented; expectations mirror the documented behaviour of the underlying function.
 * note:  OBSERVED (Win10 22H2 19045; raw "r == EINVAL got=0", "t.tm_year == -1 got=69"): _o__gmtime64_s(-1) succeeds and yields
 *        1969-12-31. Class A: the 1970 lower bound is only the legacy-CRT rule.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

typedef int (__cdecl *tm_s_t)(struct tm *, const long long *);
TC_CASE(gmtime_minus_one)
{
    struct tm t; long long v = -1; int r;
    tm_s_t f = (tm_s_t)tc_crt_proc("_o__gmtime64_s");
    if (!f) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    memset(&t, 0, sizeof t);
    r = f(&t, &v);
    CHECK_EQ_I(r, 0);          /* observed Win10 22H2: UCRT accepts pre-1970 values (same as _gmtime64_s(-1)) */
    CHECK_EQ_I(t.tm_year, 69); CHECK_EQ_I(t.tm_mon, 11); CHECK_EQ_I(t.tm_mday, 31);
}
int main(void)
{
    tc_begin();
    TC_RUN(gmtime_minus_one);
    return tc_end();
}
