/* TC
 * id:    pfm_2_wide_vfamily_conv_matrix
 * area:  stdio_printf
 * funcs: _vsnwprintf vfwprintf
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vsnwprintf, vfwprintf; printf type field characters
 * note:  vswprintf(buf,count,fmt,ap) is exercised on ucrtbase only (older CRT headers use other signatures).
 *        vfwprintf writes to a text-mode ("w", ANSI) file; ASCII content so the multibyte conversion is the identity.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#define XS_(x) #x
#define XS(x) XS_(x)
#define FN "tc_pfm_2_wide_vfamily_conv_matrix_" XS(TC_CRT_VER) ".tmp"
static volatile int vm42 = -42, v7 = 7, v8 = 8, v255 = 255, vc = 'a';
static volatile unsigned vbig = 4000000000u;
static volatile double v15 = 1.5;
static const wchar_t *volatile ws = L"cd";
static const char *volatile ns = "ef";

static int vn(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnwprintf(b, n, f, ap); va_end(ap); return r; }
#if TC_CRT_VER >= 140
static int vs(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vswprintf(b, n, f, ap); va_end(ap); return r; }
#endif
static int vf(FILE *s, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vfwprintf(s, f, ap); va_end(ap); return r; }

TC_CASE(ints)
{
    wchar_t b[64]; int r = vn(b, 64, L"%d|%i|%u|%o|%x|%X", vm42, v7, vbig, v8, v255, v255);
    CHECK_WSTR(b, L"-42|7|4000000000|10|ff|FF"); CHECK_EQ_I(r, 25);
}
TC_CASE(chars_strings)
{
    wchar_t b[64]; int r = vn(b, 64, L"%c%C%s%S", vc, vc + 1, ws, ns);
    CHECK_WSTR(b, L"abcdef"); CHECK_EQ_I(r, 6);
}
TC_CASE(floats)
{
    wchar_t b[64]; int r = vn(b, 64, L"%f|%g", v15, v15);
    CHECK_WSTR(b, L"1.500000|1.5"); CHECK_EQ_I(r, 12);
}
TC_CASE(exp_digits)
{
    wchar_t b[64]; int r = vn(b, 64, L"%e", v15);
#if TC_CRT_VER >= 140
    CHECK_WSTR(b, L"1.500000e+00"); CHECK_EQ_I(r, 12);
#else
    CHECK_WSTR(b, L"1.500000e+000"); CHECK_EQ_I(r, 13);
#endif
}
TC_CASE(pointer)
{
    wchar_t b[64]; int r = vn(b, 64, L"%p", (void *)(size_t)0x1234);
    CHECK_WSTR(b, TC_X64 ? L"0000000000001234" : L"00001234"); CHECK_EQ_I(r, TC_X64 ? 16 : 8);
}
TC_CASE(count_truncate)
{
    wchar_t b[8]; int r = vn(b, 3, L"%d", 12345);
    CHECK_EQ_I(r, -1); CHECK(b[0] == L'1' && b[1] == L'2' && b[2] == L'3');
}
TC_CASE(vswprintf_ucrt)
{
#if TC_CRT_VER >= 140
    wchar_t b[64]; int r = vs(b, 64, L"%d|%x|%ls|%.1f", vm42, v255, ws, v15);
    CHECK_WSTR(b, L"-42|ff|cd|1.5"); CHECK_EQ_I(r, 13);
    r = vs(b, 4, L"%d", 12345);
    CHECK(r < 0 || r == 5);
    CHECK_EQ_I(b[3], 0);
#else
    TC_SKIP("ucrt only");
#endif
}
TC_CASE(vfwprintf_file)
{
    FILE *f = fopen(FN, "w"); char buf[64]; size_t n = 0; int r;
    CHECK(f != NULL); if (!f) return;
    r = vf(f, L"%d|%x|%s|%S|%.2f", vm42, v255, ws, ns, v15);
    fclose(f);
    f = fopen(FN, "rb"); CHECK(f != NULL);
    if (f) { n = fread(buf, 1, sizeof buf - 1, f); fclose(f); }
    buf[n] = 0;
    CHECK_EQ_I(r, 17); CHECK_EQ_U(n, 17); CHECK_STR(buf, "-42|ff|cd|ef|1.50");
    remove(FN);
}
int main(void)
{
    tc_begin();
    TC_RUN(ints); TC_RUN(chars_strings); TC_RUN(floats); TC_RUN(exp_digits); TC_RUN(pointer); TC_RUN(count_truncate); TC_RUN(vswprintf_ucrt); TC_RUN(vfwprintf_file);
    return tc_end();
}
