/* TC
 * id:    pfq_neg_sprintf_p_count_huge
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _sprintf_p (count is buffer capacity including the terminator; -1 on error)
 * note:  PROBE: capacities 1..3 cannot hold "abc"+NUL so the call returns -1; capacity >= 4 returns 3. Whenever a call succeeds its result is <= count-1 and the terminator is at buf[result]. Count 0 -> -1 is its own case (also probed in pfq_neg_probe_sprintf_p_zero_size). Guard bytes beyond count must survive. Extreme counts (INT_MAX+1, SIZE_MAX) are not exercised: the caller is trusted with the real size.
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
static const char * volatile F = "%1$s";
static const char * volatile Xs = "x";

TC_CASE(count_one_nonempty)
{
    char b[16]; int r;
    fillc(b, 16); tc_iph_count = 0;
    r = SP(b, 1, F, Xs);
    CHECK_EQ_I(r, -1);
    CHECK(guardc(b, 1, 16));
}
TC_CASE(count_zero)
{
    char b[16]; int r;
    fillc(b, 16);
    r = SP(b, 0, F, Xs);
    CHECK_EQ_I(r, -1);
    CHECK(guardc(b, 0, 16));
}
TC_CASE(sweep_counts)
{
    char b[16]; int n, r;
    for (n = 1; n <= 8; n++) {
        fillc(b, 16);
        r = SP(b, (size_t)n, "%1$s", "abc");
        if (n < 4) CHECK_EQ_I(r, -1);
        else { CHECK_EQ_I(r, 3); CHECK_STR(b, "abc"); }
        if (r >= 0) { CHECK(r <= n - 1); CHECK_EQ_I(b[r], 0); }
        CHECK(guardc(b, n, 16));
    }
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(count_one_nonempty); TC_RUN(count_zero); TC_RUN(sweep_counts);
    return tc_end();
}
