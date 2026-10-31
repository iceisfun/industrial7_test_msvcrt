/* TC
 * id:    pfq_neg_sprintf_p_missing_dollar_garbage
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.21.6.1 (width digits) for %1d; MS docs printf_p positional parameters for the rest
 * note:  PROBE: %1d is an ordinary width-1 conversion (control, standard). Malformed positional tokens %$d and %1$ (format ends after the $) return -1 with the handler. %-1$d and %1$% are only checked for 'no crash and consistent iph': either -1 with handler, or a non-negative result.
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
#else
#define SP(b, n, ...) _sprintf_p(b, n, __VA_ARGS__)
#endif
static const char * volatile Fwidth = "%1d";
static const char * volatile Fnonum = "%$d";
static const char * volatile Fneg = "%-1$d";
static const char * volatile Fend = "%1$";
static const char * volatile Fpct = "%1$%";

TC_CASE(width_not_positional)
{
    char b[16]; int r;
    tc_iph_count = 0;
    r = SP(b, 16, Fwidth, 5);
    CHECK_EQ_I(r, 1); CHECK_STR(b, "5");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(dollar_without_number)
{
    char b[16]; int r;
    fillc(b, 16); tc_iph_count = 0;
    r = SP(b, 8, Fnonum, 5);
    CHECK_EQ_I(r, -1);
    CHECK(tc_iph_count >= 1);
    CHECK(guardc(b, 8, 16));
}
TC_CASE(ends_after_dollar)
{
    char b[16]; int r;
    fillc(b, 16); tc_iph_count = 0;
    r = SP(b, 8, Fend, 5);
    CHECK_EQ_I(r, -1);
    CHECK(tc_iph_count >= 1);
    CHECK(guardc(b, 8, 16));
}
TC_CASE(negative_position_no_crash)
{
    char b[16]; int r;
    fillc(b, 16); tc_iph_count = 0;
    r = SP(b, 8, Fneg, 5);
    CHECK(r == -1 || r >= 0);
    if (r == -1) CHECK(tc_iph_count >= 1);
    CHECK(guardc(b, 8, 16));
}
TC_CASE(percent_after_dollar_no_crash)
{
    char b[16]; int r;
    fillc(b, 16); tc_iph_count = 0;
    r = SP(b, 8, Fpct, 5);
    CHECK(r == -1 || r >= 0);
    if (r == -1) CHECK(tc_iph_count >= 1);
    CHECK(guardc(b, 8, 16));
}
TC_CASE(iph_counts_each)
{
    char b[16]; int r;
    tc_iph_count = 0;
    r = SP(b, 16, Fnonum, 5); CHECK_EQ_I(r, -1);
    r = SP(b, 16, Fend, 5); CHECK_EQ_I(r, -1);
    CHECK(tc_iph_count >= 2);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(width_not_positional); TC_RUN(dollar_without_number); TC_RUN(ends_after_dollar); TC_RUN(negative_position_no_crash); TC_RUN(percent_after_dollar_no_crash); TC_RUN(iph_counts_each);
    return tc_end();
}
