/* TC
 * id:    pfq_neg_fprintf_p_closed_fd
 * area:  stdio_printf
 * funcs: _fprintf_p _vfprintf_p
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _close, fflush (EOF on failure), errno EBADF for an invalid file descriptor
 * note:  PROBE: after _close(_fileno(f)) the stream's descriptor is dead. Buffered case: the positional write is accepted into the buffer (return 1 or -1 both tolerated) and fflush must then fail with EOF/EBADF and set ferror. Unbuffered case: the write itself fails with -1/EBADF. The EBADF values and ferror are the guesses.
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


static const char *FN = "tc_pfq_neg_fprintf_p_closed_fd.tmp";
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

static FILE *openw(void)
{
    FILE *f = fopen(FN, "w");
    return f;
}
TC_CASE(buffered_flush_fails)
{
    FILE *f; int r, fr;
    f = openw();
    if (!f) TC_SKIP("cannot open temp file");
    _close(_fileno(f));
    errno = 0;
    r = tp_f(f, "%1$d", 5);
    CHECK(r == 1 || r == -1);
    errno = 0;
    fr = fflush(f);
    if (r == 1) {  /* data was accepted into the buffer, so flushing it must fail */
        CHECK_EQ_I(fr, EOF);
        CHECK_EQ_I(errno, EBADF);
        CHECK(ferror(f) != 0);
    }
    clearerr(f);
    fclose(f);
    DeleteFileA(FN);
}
TC_CASE(unbuffered_write_fails)
{
    FILE *f; int r;
    f = openw();
    if (!f) TC_SKIP("cannot open temp file");
    CHECK_EQ_I(setvbuf(f, NULL, _IONBF, 0), 0);
    _close(_fileno(f));
    errno = 0;
    r = tp_f(f, "%1$d", 5);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
    fclose(f);
    DeleteFileA(FN);
}
TC_CASE(unbuffered_two_args)
{
    FILE *f; int r;
    f = openw();
    if (!f) TC_SKIP("cannot open temp file");
    setvbuf(f, NULL, _IONBF, 0);
    _close(_fileno(f));
    r = tp_vfn(f, "%2$s=%1$d", 5, "k");
    CHECK_EQ_I(r, -1);
    fclose(f);
    DeleteFileA(FN);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(buffered_flush_fails); TC_RUN(unbuffered_write_fails); TC_RUN(unbuffered_two_args);
    return tc_end();
}
