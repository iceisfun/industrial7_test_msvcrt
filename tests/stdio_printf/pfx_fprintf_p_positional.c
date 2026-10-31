/* TC
 * id:    pfx_fprintf_p_positional
 * area:  stdio_printf
 * funcs: _fprintf_p fopen fclose
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf_p, _printf_p, _printf_p_l, _fprintf_p (positional parameters)
 * note:  ucrtbase has no _fprintf_p export: the SDK header defines it inline over __stdio_common_vfprintf_p, replicated below.
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
int __cdecl __stdio_common_vfprintf_p(unsigned __int64, FILE *, const char *, _locale_t, va_list);
static int pf_fprintf_p(FILE *f, const char *fmt, ...)
{ int r; va_list ap; va_start(ap, fmt); r = __stdio_common_vfprintf_p(0ULL, f, fmt, NULL, ap); va_end(ap); return r; }
#else
__declspec(dllimport) int __cdecl _fprintf_p(FILE *, const char *, ...);
#define pf_fprintf_p _fprintf_p
#endif

#define FN "tc_pfx_fprintf_p_positional_a_" TC_CRT_DLL ".tmp"

TC_CASE(swap_args)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w"); int r, n;
    CHECK(f != NULL); if (!f) return;
    r = pf_fprintf_p(f, "%2$s %1$s", "hello", "world");
    fclose(f);
    CHECK_EQ_I(r, 11);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 11); CHECK_MEM(b, "world hello", 11);
    remove(FN);
}
TC_CASE(same_arg_twice)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w"); int r, n; volatile int v = 7;
    CHECK(f != NULL); if (!f) return;
    r = pf_fprintf_p(f, "%1$d%1$d", (int)v);
    fclose(f);
    CHECK_EQ_I(r, 2);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 2); CHECK_MEM(b, "77", 2);
    remove(FN);
}
TC_CASE(mixed_types_count)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w"); int r, n; volatile int v = 42;
    CHECK(f != NULL); if (!f) return;
    r = pf_fprintf_p(f, "%3$s=%2$d,%1$c", 'x', (int)v, "k");
    fclose(f);
    CHECK_EQ_I(r, 6);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 6); CHECK_MEM(b, "k=42,x", 6);
    remove(FN);
}
int main(void) { tc_begin(); remove(FN); TC_RUN(swap_args); TC_RUN(same_arg_twice); TC_RUN(mixed_types_count); return tc_end(); }
