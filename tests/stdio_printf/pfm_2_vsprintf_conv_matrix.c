/* TC
 * id:    pfm_2_vsprintf_conv_matrix
 * area:  stdio_printf
 * funcs: vsprintf _vsnprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vsprintf, _vsnprintf; printf type field characters (narrow printf: %C wide char, %S wide string)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

static volatile int v7 = 7, v8 = 8, v255 = 255;
static volatile wchar_t vwc = L'q';
static volatile double v15 = 1.5, v25 = 26.0;
static const wchar_t *volatile ws = L"wide";
static void *volatile vp = (void *)(size_t)0xABCD;

static int vs(char *b, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = vsprintf(b, f, ap); va_end(ap); return r; }
static int vn(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(vsprintf_ints_chars)
{
    char b[64]; int r = vs(b, "%i|%o|%X|%C|%S", v7, v8, v255, (int)vwc, ws);
    CHECK_STR(b, "7|10|FF|q|wide"); CHECK_EQ_I(r, 14);
}
TC_CASE(vnsprintf_ints_chars)
{
    char b[64]; int r = vn(b, 64, "%i|%o|%X|%C|%S", v7, v8, v255, (int)vwc, ws);
    CHECK_STR(b, "7|10|FF|q|wide"); CHECK_EQ_I(r, 14);
}
TC_CASE(vsprintf_floats_upper)
{
    char b[64]; int r = vs(b, "%G|%g", v25, v15);
    CHECK_STR(b, "26|1.5"); CHECK_EQ_I(r, 6);
    r = vs(b, "%.1f|%.0e", v15, v25);
#if TC_CRT_VER >= 140
    CHECK_STR(b, "1.5|3e+01"); CHECK_EQ_I(r, 9);
#else
    CHECK_STR(b, "1.5|3e+001"); CHECK_EQ_I(r, 10);
#endif
}
TC_CASE(vsprintf_E_exponent)
{
    char b[64]; int r = vs(b, "%E", v15);
#if TC_CRT_VER >= 140
    CHECK_STR(b, "1.500000E+00"); CHECK_EQ_I(r, 12);
#else
    CHECK_STR(b, "1.500000E+000"); CHECK_EQ_I(r, 13);
#endif
}
TC_CASE(vsprintf_pointer)
{
    char b[64]; int r = vs(b, "%p", vp);
    CHECK_STR(b, TC_X64 ? "000000000000ABCD" : "0000ABCD"); CHECK_EQ_I(r, TC_X64 ? 16 : 8);
}
TC_CASE(vnsprintf_count)
{
    char b[16]; int r = vn(b, 4, "%s", "abcdef");
    CHECK_EQ_I(r, -1); CHECK(b[0] == 'a' && b[1] == 'b' && b[2] == 'c' && b[3] == 'd');
    r = vn(b, 16, "%s", "abcdef"); CHECK_EQ_I(r, 6); CHECK_STR(b, "abcdef");
}
int main(void)
{
    tc_begin();
    TC_RUN(vsprintf_ints_chars); TC_RUN(vnsprintf_ints_chars); TC_RUN(vsprintf_floats_upper); TC_RUN(vsprintf_E_exponent);
    TC_RUN(vsprintf_pointer); TC_RUN(vnsprintf_count);
    return tc_end();
}
