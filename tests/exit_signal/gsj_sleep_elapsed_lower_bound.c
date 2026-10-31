/* TC
 * id:    gsj_sleep_elapsed_lower_bound
 * area:  exit_signal
 * funcs:
 * procs: _sleep
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _sleep (deprecated wrapper over Sleep)
 * note:  Loose lower bounds only (60 ms for 120 ms); no upper bound, to avoid scheduling flake.
 */
#include "tc.h"

typedef void (__cdecl *sleep_t)(unsigned long);
static sleep_t p_sleep;

static double now_ms(void)
{
    LARGE_INTEGER f, c; QueryPerformanceFrequency(&f); QueryPerformanceCounter(&c);
    return (double)c.QuadPart * 1000.0 / (double)f.QuadPart;
}
TC_CASE(zero) { if (!p_sleep) TC_SKIP("no _sleep"); p_sleep(0); CHECK(1); }
TC_CASE(lower_bound) { double t; if (!p_sleep) TC_SKIP("no _sleep"); t = now_ms(); p_sleep(120); t = now_ms() - t; CHECK(t >= 60.0); }
TC_CASE(one_twice) { if (!p_sleep) TC_SKIP("no _sleep"); p_sleep(1); p_sleep(1); CHECK(1); }
TC_CASE(ulong_arg) { volatile unsigned long ms = 50; double t; if (!p_sleep) TC_SKIP("no _sleep"); t = now_ms(); p_sleep(ms); t = now_ms() - t; CHECK(t >= 25.0); }
int main(void)
{
    tc_begin(); p_sleep = (sleep_t)tc_crt_proc("_sleep");
    TC_RUN(zero); TC_RUN(lower_bound); TC_RUN(one_twice); TC_RUN(ulong_arg);
    return tc_end();
}
