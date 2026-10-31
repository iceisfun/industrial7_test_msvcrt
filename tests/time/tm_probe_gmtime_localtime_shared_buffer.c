/* TC
 * id:    tm_probe_gmtime_localtime_shared_buffer
 * area:  time
 * funcs: _gmtime64 _localtime64 _putenv _tzset
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs localtime/gmtime remarks (shared static buffer)
 * note:  PROBE: whether _gmtime64 and _localtime64 return the SAME per-thread tm buffer, so a later call to either overwrites
 *        the other's result. Believed true (both use the per-thread gmtime buffer) but not stated unambiguously by the docs.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
static __time64_t g(__time64_t v) { volatile __time64_t x = v; return x; }
TC_CASE(gm_then_local_same_ptr)
{
    __time64_t a = g(0), b = g(86400);
    struct tm *p, *q;
    _putenv("TZ=UTC0"); _tzset();
    p = _gmtime64(&a); q = _localtime64(&b);
    CHECK_EQ_P(p, q);
    if (q) { CHECK_EQ_I(q->tm_year, 70); CHECK_EQ_I(q->tm_mon, 0); CHECK_EQ_I(q->tm_mday, 2); }
    _putenv("TZ="); _tzset();
}
TC_CASE(local_then_gm_latest_wins)
{
    __time64_t a = g(0), b = g(951782400LL);
    struct tm *p, *q;
    _putenv("TZ=UTC0"); _tzset();
    p = _localtime64(&a); q = _gmtime64(&b);
    CHECK_EQ_P(p, q);
    if (p) { CHECK_EQ_I(p->tm_year, 100); CHECK_EQ_I(p->tm_mon, 1); CHECK_EQ_I(p->tm_mday, 29); }
    _putenv("TZ="); _tzset();
}
int main(void) { tc_begin(); TC_RUN(gm_then_local_same_ptr); TC_RUN(local_then_gm_latest_wins); return tc_end(); }
