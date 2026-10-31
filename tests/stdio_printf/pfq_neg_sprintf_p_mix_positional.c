/* TC
 * id:    pfq_neg_sprintf_p_mix_positional
 * area:  stdio_printf
 * funcs: _vsprintf_p
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf_p positional parameters (positional and non-positional specifications cannot be mixed in one format string)
 * note:  PROBE: mixing styles returns -1, errno EINVAL, handler invoked. Each variant in its own case.
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


static void bad(const char *fmt)
{
    char b[32]; int r;
    fillc(b, 32); tc_iph_count = 0; errno = 0;
    r = tp_sn(b, sizeof b, fmt, 1, 2, 3, 4);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}

static const char * volatile Fa = "%1$d %d", * volatile Fb = "%d %2$d", * volatile Fc = "%1$d %*d", * volatile Fd = "%1$*d";
TC_CASE(pos_then_plain) { bad(Fa); }
TC_CASE(plain_then_pos) { bad(Fb); }
TC_CASE(pos_then_star) { bad(Fc); }
TC_CASE(star_without_position) { bad(Fd); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(pos_then_plain); TC_RUN(plain_then_pos); TC_RUN(pos_then_star); TC_RUN(star_without_position);
    return tc_end();
}
