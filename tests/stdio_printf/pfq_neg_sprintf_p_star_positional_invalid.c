/* TC
 * id:    pfq_neg_sprintf_p_star_positional_invalid
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf_p positional parameters (width/precision via *n$); positional and non-positional cannot be mixed
 * note:  PROBE: whether %1$*2$d (positional width) is supported at all is itself unverified (see pfo_probe_vsprintf_p_star_width); the control case is its own case. Invalid forms: %1$*d (non-positional star in positional format), %1$*3$d with position 2 never used, %1$.*0$d (position 0) must return -1 with the handler invoked.
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
static const char * volatile Fok = "%1$*2$d";
static const char * volatile Fmix = "%1$*d";
static const char * volatile Fgap = "%1$*3$d";
static const char * volatile Fzero = "%1$.*0$d";

static void bad(const char *fmt)
{
    char b[32]; int r;
    fillc(b, 32); tc_iph_count = 0;
    r = SP(b, 16, fmt, 5, 3, 4);
    CHECK_EQ_I(r, -1);
    CHECK(tc_iph_count >= 1);
    CHECK(guardc(b, 16, 32));
}
TC_CASE(control_star_width)
{
    char b[32]; int r;
    tc_iph_count = 0;
    r = SP(b, 32, Fok, 5, 3);
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "  5");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(star_without_position) { bad(Fmix); }
TC_CASE(star_gap) { bad(Fgap); }
TC_CASE(star_precision_zero_position) { bad(Fzero); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(control_star_width); TC_RUN(star_without_position); TC_RUN(star_gap); TC_RUN(star_precision_zero_position);
    return tc_end();
}
