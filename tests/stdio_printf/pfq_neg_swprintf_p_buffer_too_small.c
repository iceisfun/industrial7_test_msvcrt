/* TC
 * id:    pfq_neg_swprintf_p_buffer_too_small
 * area:  stdio_printf
 * funcs: _swprintf_p _vswprintf_p
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _swprintf_p / _vswprintf_p (return -1 on error; count is capacity in wide chars including the terminator)
 * note:  Only return == -1 and no write past count are asserted. errno / handler / buf[0] state on truncation are in pfq_probe_neg_sprintf_p_small_buffer_state. %1$ls is used so the argument type does not depend on narrow/wide mode.
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

static const wchar_t * volatile F = L"%1$ls";
static const wchar_t * volatile S6 = L"abcdef";

TC_CASE(count_4_small)
{
    wchar_t b[16]; int r;
    fillw(b, 16);
    r = tp_swn(b, 4, F, S6);
    CHECK_EQ_I(r, -1);
    CHECK(guardw(b, 4, 16));
}
TC_CASE(count_len_plus_one_ok)
{
    wchar_t b[16]; int r;
    fillw(b, 16);
    r = tp_swn(b, 7, F, S6);
    CHECK_EQ_I(r, 6);
    CHECK_WSTR(b, L"abcdef");
    CHECK(guardw(b, 7, 16));
}
TC_CASE(v_wrapper_small)
{
    wchar_t b[16]; int r;
    fillw(b, 16);
    r = tp_vswn(b, 3, F, S6);
    CHECK_EQ_I(r, -1);
    CHECK(guardw(b, 3, 16));
    fillw(b, 16);
    r = tp_vswn(b, 7, F, S6);
    CHECK_EQ_I(r, 6);
    CHECK_WSTR(b, L"abcdef");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(count_4_small); TC_RUN(count_len_plus_one_ok); TC_RUN(v_wrapper_small);
    return tc_end();
}
