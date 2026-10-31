/* TC
 * id:    pfq_neg_fprintf_p_readonly_stream
 * area:  stdio_printf
 * funcs: _vfprintf_p
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.21.6.1 (negative value on output error; writing to an input-only stream is UB in ISO C, Windows CRT defines it as failure); MS docs fprintf_p
 * note:  A stream opened with mode "r" cannot be written; a negative return is asserted, errno/handler state is in pfq_probe_fprintf_p_readonly_errno. ferror is probed in pfq_neg_probe_fprintf_p_readonly_errno.
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


static const char *FN = "tc_pfq_neg_fprintf_p_readonly_stream.tmp";
static int mkfile(void)
{
    DWORD n; HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return 0;
    WriteFile(h, "hello", 5, &n, NULL); CloseHandle(h); return 1;
}
static DWORD slurp(char *out, DWORD cap)
{
    DWORD n = 0; HANDLE h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return 0xffffffffu;
    if (!ReadFile(h, out, cap - 1, &n, NULL)) n = 0;
    out[n] = 0; CloseHandle(h); return n;
}

TC_CASE(fprintf_p_fails)
{
    FILE *f; int r; char c[16];
    if (!mkfile()) TC_SKIP("cannot create temp file");
    f = fopen(FN, "r");
    if (!f) TC_SKIP("cannot open temp file");
    r = tp_f(f, "%1$s", "abc");
    CHECK(r < 0);
    fclose(f);
    CHECK_EQ_U(slurp(c, sizeof c), 5);
    CHECK_STR(c, "hello");
    DeleteFileA(FN);
}
TC_CASE(vfprintf_p_fails)
{
    FILE *f; int r; char c[16];
    if (!mkfile()) TC_SKIP("cannot create temp file");
    f = fopen(FN, "r");
    if (!f) TC_SKIP("cannot open temp file");
    r = tp_f(f, "%2$s %1$d", 7, "xyz");
    CHECK(r < 0);
    fclose(f);
    CHECK_EQ_U(slurp(c, sizeof c), 5);
    CHECK_STR(c, "hello");
    DeleteFileA(FN);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(fprintf_p_fails); TC_RUN(vfprintf_p_fails);
    return tc_end();
}
