/* TC
 * id:    tm_neg_ctime64_edge_probe
 * area:  time
 * kind:  negative
 * funcs: _ctime64
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs ctime64 range
 * note:  PROBE: exactly one second past 3000-12-31 23:59:59 UTC (32535216000) with TZ=UTC0 is rejected (NULL), and 32535244799 (the localtime-adjusted limit of some older CRTs) is probed for rejection too. The EINVAL errno is not probed.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

static volatile __time64_t vt;

TC_CASE(one_past_max)
{
    __time64_t t; char *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset();
    vt = 32535216000LL; t = vt; r = _ctime64(&t);
    CHECK(r == NULL);
}
TC_CASE(plus_8h)
{
    __time64_t t; char *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset();
    vt = 32535244799LL; t = vt; r = _ctime64(&t);
    CHECK(r == NULL);
}
int main(void) { tc_begin(); TC_RUN(one_past_max); TC_RUN(plus_8h); return tc_end(); }
