/* TC
 * id:    m99_neg_c99_exports_missing_probe
 * area:  math_c99
 * funcs:
 * procs: cbrt lround nexttoward tgamma lgamma erf nan fma lroundf llrint nearbyint rint fmax fmin fdim exp2 erfc lgammaf tgammaf nanf expm1f log1pf cbrtf truncf roundf fmaf nexttowardf _nextafter
 * crt:   msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs "Universal C runtime routines by category" (C99 math added in VS2013 / CRT 12.0)
 * note:  PROBE: rarely-checked C99 names are not exported by pre-120 CRTs (extends m99_c99_neg_not_in_old_crt).
 *        _nextafter is exported as a control (the _nextafterf export is NOT asserted absent: x64 old CRTs plausibly export it).
 *        The float-suffixed case is separate so a wrong guess about one group does not mask another.
 */
#include "tc.h"

static void absent(const char *const *n, unsigned cnt)
{
    unsigned i;
    for (i = 0; i < cnt; i++)
        if (tc_crt_proc(n[i])) { tc_failhdr(__FILE__, __LINE__); tc_write("unexpected export: "); tc_write(n[i]); tc_write("\n"); }
}
static const char *const dbl[] = { "cbrt","lround","nexttoward","tgamma","lgamma","erf","nan","fma","llrint","nearbyint","rint","fmax","fmin","fdim","exp2","erfc" };
static const char *const flt[] = { "lroundf","lgammaf","tgammaf","nanf","expm1f","log1pf","cbrtf","truncf","roundf","fmaf","nexttowardf" };
TC_CASE(double_names_absent) { absent(dbl, sizeof dbl / sizeof dbl[0]); }
TC_CASE(float_names_absent) { absent(flt, sizeof flt / sizeof flt[0]); }
TC_CASE(nextafter_present_control)
{
    if (!TC_AT_LEAST(80)) TC_SKIP("msvcrt.dll _nextafter not probed");
    CHECK(tc_crt_proc("_nextafter") != NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(double_names_absent); TC_RUN(float_names_absent); TC_RUN(nextafter_present_control);
    return tc_end();
}
