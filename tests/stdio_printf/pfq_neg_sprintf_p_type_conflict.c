/* TC
 * id:    pfq_neg_sprintf_p_type_conflict
 * area:  stdio_printf
 * funcs: _vsprintf_p
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   assumption only: conflicting types for one position are diagnosed (not stated in MS docs)
 * note:  PROBE: (old CRTs may simply let the last type win and succeed, in which case this fails there - that is the finding) type conflicts on a repeated position return -1 (EINVAL, handler). The argument is a pointer to a string so that an unexpected success cannot crash; 'compatible_reuse' is a positive contrast.
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


static const char S[] = "x";
static const char * volatile Fa = "%1$d %1$s", * volatile Fb = "%1$d %1$f", * volatile Fc = "%1$*1$s";
static void bad1(const char *fmt)
{
    char b[32]; int r;
    fillc(b, 32); tc_iph_count = 0; errno = 0;
    r = tp_sn(b, sizeof b, fmt, (const char * volatile)S);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(int_vs_string) { bad1(Fa); }
TC_CASE(int_vs_double) { bad1(Fb); }
TC_CASE(width_vs_string) { bad1(Fc); }
TC_CASE(compatible_reuse)
{
    char b[32]; int r;
    fillc(b, 32); tc_iph_count = 0;
    r = tp_sn(b, sizeof b, "%1$d %1$x", 255);
    CHECK_EQ_I(r, 6);
    CHECK_STR(b, "255 ff");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(int_vs_string); TC_RUN(int_vs_double); TC_RUN(width_vs_string); TC_RUN(compatible_reuse);
    return tc_end();
}
