/* TC
 * id:    tm_time_basic
 * area:  time
 * funcs: _putenv _tzset
 * procs: time
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.27.2.4; MS docs time
 * note:  time may not be a DLL export on msvcr80+/ucrtbase (inline wrapper over _time64), so it is resolved at run time and cases SKIP if absent; export presence is probed in tm_time_export_probe. Only loose bounds on the value; TZ must not affect time().
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(time_t, time, (time_t *));
#define time tc_p_time


TC_CASE(range) { if (!tc_p_time) TC_SKIP("time not exported"); time_t t = time(NULL); CHECK((__time64_t)t > 1600000000LL); CHECK((__time64_t)t < 32535244800LL); }
TC_CASE(out_pointer)
{
    if (!tc_p_time) TC_SKIP("time not exported");
    time_t t = 0, r = time(&t), r2;
    CHECK(t != 0);
    r2 = time(NULL);
    CHECK((__time64_t)(r2 - t) >= 0 && (__time64_t)(r2 - t) <= 5);
    CHECK((__time64_t)(r2 - r) >= 0 && (__time64_t)(r2 - r) <= 5);
}
TC_CASE(monotone) { if (!tc_p_time) TC_SKIP("time not exported"); time_t a = time(NULL), b = time(NULL), c = time(NULL); CHECK(b >= a); CHECK(c >= b); }
TC_CASE(tz_independent)
{
    if (!tc_p_time) TC_SKIP("time not exported");
    time_t a, b;
    _putenv("TZ=UTC0"); _tzset(); a = time(NULL);
    _putenv("TZ=PST8PDT"); _tzset(); b = time(NULL);
    CHECK((__time64_t)(b - a) >= 0 && (__time64_t)(b - a) < 3);
    _putenv("TZ="); _tzset();
}
int main(void) { tc_begin(); TC_LOAD_PROC(time); TC_RUN(range); TC_RUN(out_pointer); TC_RUN(monotone); TC_RUN(tz_independent); return tc_end(); }
