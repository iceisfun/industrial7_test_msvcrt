/* TC
 * id:    pfx_stdio_common_vfprintf_direct
 * area:  stdio_printf
 * funcs: __stdio_common_vfprintf __stdio_common_vfprintf_s __stdio_common_vfprintf_p __stdio_common_vfwprintf fopen fclose
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   Windows SDK ucrt corecrt_stdio_config.h / stdio.h (declared, otherwise undocumented); the inline fprintf family calls these
 * note:  options 0 and locale NULL. The _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR (0x2) question is in
 *        pfx_stdio_common_vfprintf_opt_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

/* read whole file via kernel32 (no CRT) */
static int slurp(const char *name, unsigned char *buf, int max)
{
    DWORD n = 0; HANDLE h = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, (DWORD)max, &n, NULL)) n = 0;
    CloseHandle(h); return (int)n;
}

int __cdecl __stdio_common_vfprintf_s(unsigned __int64, FILE *, const char *, _locale_t, va_list);
int __cdecl __stdio_common_vfprintf_p(unsigned __int64, FILE *, const char *, _locale_t, va_list);

#define FN "tc_pfx_stdio_common_vfprintf_direct_a_" TC_CRT_DLL ".tmp"

static int c_plain(FILE *f, const char *fmt, ...)
{ int r; va_list ap; va_start(ap, fmt); r = __stdio_common_vfprintf(0, f, fmt, NULL, ap); va_end(ap); return r; }
static int c_s(FILE *f, const char *fmt, ...)
{ int r; va_list ap; va_start(ap, fmt); r = __stdio_common_vfprintf_s(0, f, fmt, NULL, ap); va_end(ap); return r; }
static int c_p(FILE *f, const char *fmt, ...)
{ int r; va_list ap; va_start(ap, fmt); r = __stdio_common_vfprintf_p(0, f, fmt, NULL, ap); va_end(ap); return r; }
static int c_w(FILE *f, const wchar_t *fmt, ...)
{ int r; va_list ap; va_start(ap, fmt); r = __stdio_common_vfwprintf(0, f, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(plain)
{
    unsigned char b[32]; FILE *f = fopen(FN, "w"); int r, n; volatile int v = 12;
    CHECK(f != NULL); if (!f) return;
    r = c_plain(f, "x=%d", (int)v); fclose(f);
    CHECK_EQ_I(r, 4);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 4); CHECK_MEM(b, "x=12", 4);
    remove(FN);
}
TC_CASE(secure_variant)
{
    unsigned char b[32]; FILE *f = fopen(FN, "w"); int r, n; volatile int v = 12;
    CHECK(f != NULL); if (!f) return;
    r = c_s(f, "x=%d,%s", (int)v, "ok"); fclose(f);
    CHECK_EQ_I(r, 7);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 7); CHECK_MEM(b, "x=12,ok", 7);
    remove(FN);
}
TC_CASE(positional_variant)
{
    unsigned char b[32]; FILE *f = fopen(FN, "w"); int r, n;
    CHECK(f != NULL); if (!f) return;
    r = c_p(f, "%2$s-%1$s", "a", "b"); fclose(f);
    CHECK_EQ_I(r, 3);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 3); CHECK_MEM(b, "b-a", 3);
    remove(FN);
}
TC_CASE(wide_variant)
{
    unsigned char b[32]; FILE *f = fopen(FN, "w"); int r, n; volatile int v = 9;
    CHECK(f != NULL); if (!f) return;
    r = c_w(f, L"w=%d%ls", (int)v, L"!"); fclose(f);
    CHECK_EQ_I(r, 4);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 4); CHECK_MEM(b, "w=9!", 4);
    remove(FN);
}
int main(void) { tc_begin(); remove(FN); TC_RUN(plain); TC_RUN(secure_variant); TC_RUN(positional_variant); TC_RUN(wide_variant); return tc_end(); }
