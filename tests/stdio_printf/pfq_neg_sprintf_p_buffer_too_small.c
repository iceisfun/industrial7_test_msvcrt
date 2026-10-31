/* TC
 * id:    pfq_neg_sprintf_p_buffer_too_small
 * area:  stdio_printf
 * funcs: _vsprintf_p _swprintf_p
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _sprintf_p, _swprintf_p (return -1 on error; count is the buffer capacity in chars incl. NUL, nothing is written past it)
 * note:  Only return < 0 and no overrun are asserted. errno/handler/buf[0] state is in pfq_probe_neg_sprintf_p_small_buffer_state. Guard bytes are placed after the declared size.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>

#if TC_CRT_VER >= 140
static int tp_vsn(char *b, size_t n, const char *f, va_list ap) { return __stdio_common_vsprintf_p(0, b, n, f, NULL, ap); }
static int tp_vf(FILE *s, const char *f, va_list ap) { return __stdio_common_vfprintf_p(0, s, f, NULL, ap); }
static int tp_vsc(const char *f, va_list ap) { return __stdio_common_vsprintf_p(2, NULL, 0, f, NULL, ap); }
#else
int __cdecl _vsprintf_p(char *, size_t, const char *, va_list);
int __cdecl _vfprintf_p(FILE *, const char *, va_list);
int __cdecl _vscprintf_p(const char *, va_list);
static int tp_vsn(char *b, size_t n, const char *f, va_list ap) { return _vsprintf_p(b, n, f, ap); }
static int tp_vf(FILE *s, const char *f, va_list ap) { return _vfprintf_p(s, f, ap); }
static int tp_vsc(const char *f, va_list ap) { return _vscprintf_p(f, ap); }
#endif
static int tp_sn(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vsn(b, n, f, ap); va_end(ap); return r; }
static int tp_f(FILE *s, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vf(s, f, ap); va_end(ap); return r; }
static int tp_sc(const char *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vsc(f, ap); va_end(ap); return r; }
#if TC_CRT_VER >= 140
static int tp_vswn_(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return __stdio_common_vswprintf_p(0, b, n, f, NULL, ap); }
static int tp_vfw_(FILE *s, const wchar_t *f, va_list ap) { return __stdio_common_vfwprintf_p(0, s, f, NULL, ap); }
static int tp_swn(wchar_t *b, size_t n, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vswn_(b, n, f, ap); va_end(ap); return r; }
static int tp_fw(FILE *s, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vfw_(s, f, ap); va_end(ap); return r; }
static int tp_scw(const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf_p(2, NULL, 0, f, NULL, ap); va_end(ap); return r; }
#else
static int tp_vswn_(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return _vswprintf_p(b, n, f, ap); }
static int tp_vfw_(FILE *s, const wchar_t *f, va_list ap) { return _vfwprintf_p(s, f, ap); }
#define tp_swn _swprintf_p
#define tp_fw _fwprintf_p
#define tp_scw _scwprintf_p
#endif
static int tp_vswn(wchar_t *b, size_t n, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vswn_(b, n, f, ap); va_end(ap); return r; }
static int tp_vfw(FILE *s, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = tp_vfw_(s, f, ap); va_end(ap); return r; }
static void fillc(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = 0x5A; }
static int guardc(const char *b, int from, int to) { int i; for (i = from; i < to; i++) if ((unsigned char)b[i] != 0x5A) return 0; return 1; }
static void fillw(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = 0x5A5A; }
static int guardw(const wchar_t *b, int from, int to) { int i; for (i = from; i < to; i++) if (b[i] != 0x5A5A) return 0; return 1; }


TC_CASE(too_small)
{
    char b[16]; int r;
    fillc(b, 16); tc_iph_count = 0;
    r = tp_sn(b, 4, "%1$s", "abcdef");
    CHECK(r < 0);
    CHECK(guardc(b, 4, 16));
}
TC_CASE(len_plus_one_ok)
{
    char b[16]; int r;
    fillc(b, 16); tc_iph_count = 0;
    r = tp_sn(b, 7, "%1$s", "abcdef");
    CHECK_EQ_I(r, 6);
    CHECK_STR(b, "abcdef");
    CHECK(guardc(b, 7, 16));
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_too_small)
{
    wchar_t b[16]; int r;
    fillw(b, 16); tc_iph_count = 0;
    r = tp_swn(b, 4, L"%1$ls", L"abcdef");
    CHECK(r < 0);
    CHECK(guardw(b, 4, 16));
}
TC_CASE(wide_len_plus_one_ok)
{
    wchar_t b[16]; int r;
    fillw(b, 16); tc_iph_count = 0;
    r = tp_swn(b, 7, L"%1$ls", L"abcdef");
    CHECK_EQ_I(r, 6);
    CHECK_WSTR(b, L"abcdef");
    CHECK(guardw(b, 7, 16));
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(too_small); TC_RUN(len_plus_one_ok); TC_RUN(wide_too_small); TC_RUN(wide_len_plus_one_ok);
    return tc_end();
}
