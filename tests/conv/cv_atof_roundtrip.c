/* TC
 * id:    cv_atof_roundtrip
 * area:  conv
 * funcs: _gcvt
 * procs: strtod atof
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   C11 7.22.1.3 (correctly rounded decimal-to-binary); MS docs _gcvt, strtod, atof
 * note:  _gcvt with 17 significant digits always carries enough digits to round-trip a double; strtod/atof are resolved from the DLL (mingw would otherwise link its own gdtoa copy). Denormals moved to cv_gcvt_strtod_denorm_probe. Pre-UCRT strtod is not correctly rounded for every input, but 17-digit strings of exact doubles are far from any rounding boundary.
 */
#include "tc.h"

TC_DECL_PROC(double, strtod, (const char *, char **));
TC_DECL_PROC(double, atof, (const char *));
extern char *__cdecl _gcvt(double, int, char *);

static void rt(double v)
{
    char b[64]; char *e; double r;
    _gcvt(v, 17, b);
    r = tc_p_strtod(b, &e);
    CHECK_BITS_D(r, v);
    CHECK(*e == 0);
    r = tc_p_atof(b);
    CHECK_BITS_D(r, v);
}

TC_CASE(simple_values)
{
    volatile double a = 0.5, b = 0.25, c = 1024.0, d = -3.75, e = 1.0, f = -1.0e10;
    rt(a); rt(b); rt(c); rt(d); rt(e); rt(f);
}
TC_CASE(non_exact_values)
{
    volatile double a = 0.1, b = 1.0 / 3.0, c = 3.141592653589793, d = 1.0e-5, e = 123456789.123456789;
    rt(a); rt(b); rt(c); rt(d); rt(e);
}
TC_CASE(dbl_max_min)
{
    char *e; volatile double mx = 1.7976931348623157e308, mn = 2.2250738585072014e-308;
    rt(mx); rt(-mx); rt(mn);
    CHECK_BITS_D(tc_p_strtod("1.7976931348623157e308", &e), mx);
}
TC_CASE(powers_of_two)
{
    volatile double v = 1.0; int i;
    for (i = 0; i > -1022; i--) { rt(v); v = v * 0.5; }   /* 2^0 .. 2^-1021 */
    rt(v);                                                /* 2^-1022 */
    v = 1.0;
    for (i = 0; i < 1023; i++) { rt(v); v = v * 2.0; }
    rt(v);                                                /* 2^1023 */
}

int main(void)
{
    tc_begin(); TC_LOAD_PROC(strtod); TC_LOAD_PROC(atof);
    if (!tc_p_strtod || !tc_p_atof) { tc_write("TC FAIL setup\n"); return 1; }
    TC_RUN(simple_values); TC_RUN(non_exact_values); TC_RUN(dbl_max_min); TC_RUN(powers_of_two);
    return tc_end();
}
