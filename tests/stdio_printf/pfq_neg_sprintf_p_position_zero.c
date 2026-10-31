/* TC
 * id:    pfq_neg_sprintf_p_position_zero
 * area:  stdio_printf
 * funcs: _sprintf_p _sprintf_p_l
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf_p positional parameters (position is a decimal number starting at 1)
 * note:  PROBE: %0$d is rejected: -1, handler invoked, nothing written past count. _sprintf_p_l with a NULL locale (current locale) behaves the same. Controls (%1$d) are separate cases.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>
#include <io.h>

#if TC_CRT_VER >= 140
static int tp_vsn(char *b, size_t n, const char *f, va_list ap) { return __stdio_common_vsprintf_p(0, b, n, f, NULL, ap); }
static int tp_vf(FILE *s, const char *f, va_list ap) { return __stdio_common_vfprintf_p(0, s, f, NULL, ap); }
static int tp_vswn_(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return __stdio_common_vswprintf_p(0, b, n, f, NULL, ap); }
static int tp_vfw_(FILE *s, const wchar_t *f, va_list ap) { return __stdio_common_vfwprintf_p(0, s, f, NULL, ap); }
#else
int __cdecl _vsprintf_p(char *, size_t, const char *, va_list);
int __cdecl _vfprintf_p(FILE *, const char *, va_list);
static int tp_vsn(char *b, size_t n, const char *f, va_list ap) { return _vsprintf_p(b, n, f, ap); }
static int tp_vf(FILE *s, const char *f, va_list ap) { return _vfprintf_p(s, f, ap); }
static int tp_vswn_(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return _vswprintf_p(b, n, f, ap); }
static int tp_vfw_(FILE *s, const wchar_t *f, va_list ap) { return _vfwprintf_p(s, f, ap); }
#endif
#if TC_CRT_VER >= 140
static int tp_sn(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vsn(b, n, f, ap); va_end(ap); return r; }
static int tp_f(FILE *s, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vf(s, f, ap); va_end(ap); return r; }
static int tp_swn(wchar_t *b, size_t n, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vswn_(b, n, f, ap); va_end(ap); return r; }
static int tp_fw(FILE *s, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vfw_(s, f, ap); va_end(ap); return r; }
#else
#define tp_sn _sprintf_p
#define tp_f _fprintf_p
#define tp_swn _swprintf_p
#define tp_fw _fwprintf_p
#endif
/* va_list entry points, reached through variadic shims */
static int tp_vswn(wchar_t *b, size_t n, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vswn_(b, n, f, ap); va_end(ap); return r; }
static int tp_vfw(FILE *s, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vfw_(s, f, ap); va_end(ap); return r; }
static int tp_vfn(FILE *s, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vf(s, f, ap); va_end(ap); return r; }
static int tp_vsnn(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vsn(b, n, f, ap); va_end(ap); return r; }
static void fillc(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = 0x5A; }
static int guardc(const char *b, int from, int to) { int i; for (i = from; i < to; i++) if ((unsigned char)b[i] != 0x5A) return 0; return 1; }
static void fillw(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = 0x5A5A; }
static int guardw(const wchar_t *b, int from, int to) { int i; for (i = from; i < to; i++) if (b[i] != 0x5A5A) return 0; return 1; }


#if TC_CRT_VER >= 140
#define SP(b, n, ...) tp_sn(b, n, __VA_ARGS__)
#define SPL(b, n, ...) tp_sn(b, n, __VA_ARGS__)
#else
#define SP(b, n, ...) _sprintf_p(b, n, __VA_ARGS__)
#define SPL(b, n, f, ...) _sprintf_p_l(b, n, f, NULL, __VA_ARGS__)
#endif
static const char * volatile F0 = "%0$d";
static const char * volatile F0b = "x%0$d%1$d";

TC_CASE(zero_position)
{
    char b[16]; int r;
    fillc(b, 16); tc_iph_count = 0;
    r = SP(b, 8, F0, 7);
    CHECK_EQ_I(r, -1);
    CHECK(tc_iph_count >= 1);
    CHECK(guardc(b, 8, 16));
}
TC_CASE(zero_position_after_text)
{
    char b[16]; int r;
    fillc(b, 16); tc_iph_count = 0;
    r = SP(b, 8, F0b, 7, 8);
    CHECK_EQ_I(r, -1);
    CHECK(tc_iph_count >= 1);
    CHECK(guardc(b, 8, 16));
}
TC_CASE(zero_position_l_null_locale)
{
    char b[16]; int r;
    fillc(b, 16); tc_iph_count = 0;
    r = SPL(b, 8, F0, 7);
    CHECK_EQ_I(r, -1);
    CHECK(tc_iph_count >= 1);
    CHECK(guardc(b, 8, 16));
}
TC_CASE(control_position_one)
{
    char b[16]; int r;
    tc_iph_count = 0;
    r = SP(b, 16, "%1$d", 7);
    CHECK_EQ_I(r, 1); CHECK_STR(b, "7");
    r = SPL(b, 16, "%1$d", 8);
    CHECK_EQ_I(r, 1); CHECK_STR(b, "8");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(zero_position); TC_RUN(zero_position_after_text); TC_RUN(zero_position_l_null_locale); TC_RUN(control_position_one);
    return tc_end();
}
