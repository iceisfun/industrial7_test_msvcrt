/* TC
 * id:    exp_math_c99_ldouble_ucrt
 * area:  exports
 * funcs:
 * procs: acoshl asinhl cbrtl erfl expm1l fmal fmaxl lgammal log2l roundl tgammal truncl
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   UCRT math.h (C99 long double functions)
 * note:  long double == double on MSVC, so these are plain exports. sinl/powl are deliberately not claimed.
 */
#include "tc.h"
static void need(const char *const *n)
{
    for (; *n; n++) if (!tc_crt_proc(*n)) { tc_failhdr(__FILE__, __LINE__); tc_write("missing export: "); tc_write(*n); tc_write("\n"); }
}
static void absent(const char *const *n)
{
    for (; *n; n++) if (tc_crt_proc(*n)) { tc_failhdr(__FILE__, __LINE__); tc_write("unexpected export: "); tc_write(*n); tc_write("\n"); }
}

TC_CASE(exported_group)
{
    static const char *const n[] = { "acoshl", "asinhl", "cbrtl", "erfl", "expm1l", "fmal", "fmaxl", "lgammal", "log2l", "roundl", "tgammal", "truncl", 0 };
    need(n);
}
TC_CASE(roundl_callable)
{
    double (__cdecl *p)(double) = (double (__cdecl *)(double))tc_crt_proc("roundl");
    volatile double x = 2.5;
    if (!p) TC_SKIP("roundl not exported");
    CHECK_BITS_D(p(x), 3.0);
}
int main(void) { tc_begin(); TC_RUN(exported_group); TC_RUN(roundl_callable); return tc_end(); }
