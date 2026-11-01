/* TC
 * id:    pfq_probe_neg_sprintf_p_small_buffer_state
 * area:  stdio_printf
 * funcs: _vsprintf_p _swprintf_p
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _sprintf_p (secure-CRT style parameter validation)
 * note:  OBSERVED on ucrtbase (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "errno_erange errno == ERANGE got=0", "iph_once ... got=0", "buf0_zero b[0] == 0 got=97"; zzp_probe2 "_sprintf_p small
 *        (opt0): r=-2 errno=0 iph=0 b0=97", "vswprintf_p opt0 small: r=-2 errno=0 iph=0 b0=97"): _sprintf_p/_swprintf_p on a too-small buffer behave like the
 *        legacy _snprintf family (return -2, errno unchanged, no handler, first count chars copied without NUL so buf[0]=='a'), NOT like
 *        sprintf_s as originally modelled. sprintf_s-style expectations kept for pre-UCRT CRTs (unverified). Stable.
 *        NOTE: -2 is the raw ucrtbase __stdio_common_v*printf_p export result (observed, not documented); MSVC headers map any negative result to -1.
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


TC_CASE(errno_erange)
{
    char b[16]; int r;
    fillc(b, 16); tc_iph_count = 0; errno = 0;
    r = tp_sn(b, 4, "%1$s", "abcdef");
    CHECK(r < 0);
#if TC_CRT_VER >= 140
    CHECK_EQ_I(errno, 0);   /* ucrtbase observed: errno untouched */
#else
    CHECK_EQ_I(errno, ERANGE);
#endif
}
TC_CASE(iph_once)
{
    char b[16]; int r;
    fillc(b, 16); tc_iph_count = 0;
    r = tp_sn(b, 4, "%1$s", "abcdef");
    CHECK(r < 0);
#if TC_CRT_VER >= 140
    CHECK_EQ_I(tc_iph_count, 0);   /* ucrtbase observed: no handler call */
#else
    CHECK_EQ_I(tc_iph_count, 1);
#endif
}
TC_CASE(buf0_zero)
{
    char b[16]; int r;
    fillc(b, 16);
    r = tp_sn(b, 4, "%1$s", "abcdef");
    CHECK(r < 0);
#if TC_CRT_VER >= 140
    CHECK_EQ_I(b[0], 'a');   /* ucrtbase observed: partial copy, buf[0] NOT zeroed */
#else
    CHECK_EQ_I(b[0], 0);
#endif
}
TC_CASE(wide_errno_erange)
{
    wchar_t b[16]; int r;
    fillw(b, 16); errno = 0;
    r = tp_swn(b, 4, L"%1$ls", L"abcdef");
    CHECK(r < 0);
#if TC_CRT_VER >= 140
    CHECK_EQ_I(errno, 0);
#else
    CHECK_EQ_I(errno, ERANGE);
#endif
}
TC_CASE(wide_iph_once)
{
    wchar_t b[16]; int r;
    fillw(b, 16); tc_iph_count = 0;
    r = tp_swn(b, 4, L"%1$ls", L"abcdef");
    CHECK(r < 0);
#if TC_CRT_VER >= 140
    CHECK_EQ_I(tc_iph_count, 0);
#else
    CHECK_EQ_I(tc_iph_count, 1);
#endif
}
TC_CASE(wide_buf0_zero)
{
    wchar_t b[16]; int r;
    fillw(b, 16);
    r = tp_swn(b, 4, L"%1$ls", L"abcdef");
    CHECK(r < 0);
#if TC_CRT_VER >= 140
    CHECK_EQ_I(b[0], L'a');
#else
    CHECK_EQ_I(b[0], 0);
#endif
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(errno_erange); TC_RUN(iph_once); TC_RUN(buf0_zero); TC_RUN(wide_errno_erange); TC_RUN(wide_iph_once); TC_RUN(wide_buf0_zero);
    return tc_end();
}
