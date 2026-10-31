/* TC
 * id:    pfm_2_target_old_crt_basic_matrix
 * area:  stdio_printf
 * funcs: sprintf _snprintf _vsnprintf
 * procs: _scprintf
 * crt:   msvcr80 msvcr90 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   C11 7.21.6.1; MS docs printf format specification (exponent is at least three digits before VS2015 / UCRT); _snprintf, _vsnprintf, _scprintf
 * note:  Basic flags / width / precision / conversions on the oldest targets. _scprintf is resolved at run time (absent from msvcrt.dll's import library; skipped if missing).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int vsn(char *b, size_t n, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap); return r; }
TC_DECL_PROC(int, _scprintf, (const char *, ...));

TC_CASE(int_flags)
{
    char b[64]; const char * volatile f = "%+5d|%-5d|%05d|% d"; int n = sprintf(b, f, 42, 42, 42, 42);
    CHECK_STR(b, "  +42|42   |00042| 42"); CHECK_EQ_I(n, 21);
}
TC_CASE(unsigned_hex_oct)
{
    char b[64]; const char * volatile f = "%u|%x|%X|%o|%#x|%#o"; sprintf(b, f, 4000000000u, 255, 255, 8, 255, 8);
    CHECK_STR(b, "4000000000|ff|FF|10|0xff|010");
}
TC_CASE(string_precision)
{
    char b[64]; const char * volatile f = "%.3s|%5.1s|%-4s|%c"; sprintf(b, f, "abcdef", "xyz", "q", 'Z');
    CHECK_STR(b, "abc|    x|q   |Z");
}
TC_CASE(exp_three_digits)
{
    char b[64]; volatile double d = 1.5; sprintf(b, "%e", d);
    CHECK_STR(b, "1.500000e+000");
    sprintf(b, "%E|%g", 1e-5 * d, 1e20 * d);
    CHECK_STR(b, "1.500000E-005|1.5e+020");
}
TC_CASE(float_fixed_general)
{
    char b[64]; volatile double d = 2.5; sprintf(b, "%f|%.2f|%g", d, d, d);
    CHECK_STR(b, "2.500000|2.50|2.5");
}
TC_CASE(snprintf_and_vsnprintf)
{
    char b[16]; int n = _snprintf(b, sizeof b, "%d-%s", 7, "ab");
    CHECK_EQ_I(n, 4); CHECK_STR(b, "7-ab");
    n = vsn(b, sizeof b, "%x", 255); CHECK_EQ_I(n, 2); CHECK_STR(b, "ff");
}
TC_CASE(scprintf_agrees)
{
    char b[64]; const char * volatile f = "%+5d|%.3s|%e"; int n, m;
    if (!tc_p__scprintf) TC_SKIP("_scprintf absent");
    n = sprintf(b, f, 42, "abcdef", 1.5);
    m = tc_p__scprintf(f, 42, "abcdef", 1.5);
    CHECK_EQ_I(m, n); CHECK_EQ_I(n, 5 + 1 + 3 + 1 + 13);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_scprintf);
    TC_RUN(int_flags); TC_RUN(unsigned_hex_oct); TC_RUN(string_precision); TC_RUN(exp_three_digits);
    TC_RUN(float_fixed_general); TC_RUN(snprintf_and_vsnprintf); TC_RUN(scprintf_agrees);
    return tc_end();
}
