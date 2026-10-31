/* TC
 * id:    exp_neg_o_time_s_negative_probe
 * area:  exports
 * funcs:
 * procs: _o__gmtime64_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _gmtime64_s (range starts at 1970-01-01)
 * note:  PROBE: _o__gmtime64_s(-1): docs give a 1970 lower bound; whether UCRT 10 still rejects pre-1970 values is what is measured (expected EINVAL, fields -1).
 *        The _o_ aliases are undocumented; expectations mirror the documented behaviour of the underlying function.
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
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(t.tm_year, -1);
}
int main(void)
{
    tc_begin();
    TC_RUN(gmtime_minus_one);
    return tc_end();
}
