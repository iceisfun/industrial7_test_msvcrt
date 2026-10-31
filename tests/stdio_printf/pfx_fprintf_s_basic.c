/* TC
 * id:    pfx_fprintf_s_basic
 * area:  stdio_printf
 * funcs: fprintf_s fopen fclose
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fprintf_s, _fprintf_s_l (same as fprintf for valid input); fopen text mode
 * note:  ucrtbase has no fprintf_s export: the SDK header defines it inline over __stdio_common_vfprintf_s, replicated below.
 *        Only the file contents are asserted for the text-mode newline case, not the return count (see pfx_fprintf_crlf_count_probe).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

/* read whole file via kernel32 (no CRT) */
static int slurp(const char *name, unsigned char *buf, int max)
{
    DWORD n = 0; HANDLE h = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, (DWORD)max, &n, NULL)) n = 0;
    CloseHandle(h); return (int)n;
}

#if TC_CRT_VER >= 140
int __cdecl __stdio_common_vfprintf_s(unsigned __int64, FILE *, const char *, _locale_t, va_list);
static int pf_fprintf_s(FILE *f, const char *fmt, ...)
{ int r; va_list ap; va_start(ap, fmt); r = __stdio_common_vfprintf_s(0ULL, f, fmt, NULL, ap); va_end(ap); return r; }
#else
__declspec(dllimport) int __cdecl fprintf_s(FILE *, const char *, ...);
#define pf_fprintf_s fprintf_s
#endif

#define FN "tc_pfx_fprintf_s_basic_a_" TC_CRT_DLL ".tmp"

TC_CASE(ints_strings_chars)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w"); int r, n; volatile int v = -12;
    CHECK(f != NULL); if (!f) return;
    r = pf_fprintf_s(f, "%d|%s|%c|%5.1f|%x", (int)v, "str", 'Z', 2.5, 255u);
    fclose(f);
    CHECK_EQ_I(r, 18);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 18); CHECK_MEM(b, "-12|str|Z|  2.5|ff", 18);
    remove(FN);
}
TC_CASE(text_mode_newline_bytes)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w"); int n;
    CHECK(f != NULL); if (!f) return;
    pf_fprintf_s(f, "a\n%s\n", "b");
    fclose(f);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 6); CHECK_MEM(b, "a\r\nb\r\n", 6);
    remove(FN);
}
TC_CASE(binary_mode_newline_bytes)
{
    unsigned char b[64]; FILE *f = fopen(FN, "wb"); int n, r;
    CHECK(f != NULL); if (!f) return;
    r = pf_fprintf_s(f, "a\n%s\n", "b");
    fclose(f);
    CHECK_EQ_I(r, 4);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 4); CHECK_MEM(b, "a\nb\n", 4);
    remove(FN);
}
int main(void) { tc_begin(); remove(FN); TC_RUN(ints_strings_chars); TC_RUN(text_mode_newline_bytes); TC_RUN(binary_mode_newline_bytes); return tc_end(); }
