/* TC
 * id:    pfx_fprintf_l_basic
 * area:  stdio_printf
 * funcs: _fprintf_l _fwprintf_l _vfprintf_l _create_locale _free_locale fopen fclose
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf, _printf_l, wprintf, _wprintf_l (fprintf, _fprintf_l); _create_locale
 * note:  ucrtbase has no _fprintf_l export: the SDK header defines it inline over __stdio_common_vfprintf, replicated below.
 *        msvcr80/90/msvcrt omitted: the mingw import libraries lack the symbols (_create_locale etc.).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <locale.h>

/* read whole file via kernel32 (no CRT) */
static int slurp(const char *name, unsigned char *buf, int max)
{
    DWORD n = 0; HANDLE h = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, (DWORD)max, &n, NULL)) n = 0;
    CloseHandle(h); return (int)n;
}

#if TC_CRT_VER >= 140
int __cdecl __stdio_common_vfprintf(unsigned __int64, FILE *, const char *, _locale_t, va_list);
static int pf_vfprintf_l(FILE *f, const char *fmt, _locale_t l, va_list ap) { return __stdio_common_vfprintf(0ULL, f, fmt, l, ap); }
static int pf_fprintf_l(FILE *f, const char *fmt, _locale_t l, ...)
{ int r; va_list ap; va_start(ap, l); r = pf_vfprintf_l(f, fmt, l, ap); va_end(ap); return r; }
int __cdecl __stdio_common_vfwprintf(unsigned __int64, FILE *, const wchar_t *, _locale_t, va_list);
static int pf_fwprintf_l(FILE *f, const wchar_t *fmt, _locale_t l, ...)
{ int r; va_list ap; va_start(ap, l); r = __stdio_common_vfwprintf(0ULL, f, fmt, l, ap); va_end(ap); return r; }
#else
__declspec(dllimport) int __cdecl _fprintf_l(FILE *, const char *, _locale_t, ...);
__declspec(dllimport) int __cdecl _vfprintf_l(FILE *, const char *, _locale_t, va_list);
#define pf_fprintf_l _fprintf_l
#define pf_vfprintf_l _vfprintf_l
#define pf_fwprintf_l _fwprintf_l
#endif

#define FN "tc_pfx_fprintf_l_basic_a_" TC_CRT_DLL ".tmp"

static int vwrap(FILE *f, const char *fmt, _locale_t l, ...)
{ int r; va_list ap; va_start(ap, l); r = pf_vfprintf_l(f, fmt, l, ap); va_end(ap); return r; }

TC_CASE(explicit_c_locale)
{
    unsigned char b[64]; _locale_t l = _create_locale(LC_ALL, "C"); FILE *f; int r, n; volatile double d = 1.5; volatile int i = 5;
    CHECK(l != NULL); if (!l) return;
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) { _free_locale(l); return; }
    r = pf_fprintf_l(f, "%d %.2f", l, (int)i, (double)d);
    fclose(f);
    CHECK_EQ_I(r, 6);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 6); CHECK_MEM(b, "5 1.50", 6);
    _free_locale(l); remove(FN);
}
TC_CASE(null_locale)
{
    unsigned char b[64]; FILE *f; int r, n; volatile double d = 1.5; volatile int i = 5;
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    r = pf_fprintf_l(f, "%d %.2f", NULL, (int)i, (double)d);
    fclose(f);
    CHECK_EQ_I(r, 6);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 6); CHECK_MEM(b, "5 1.50", 6);
    remove(FN);
}
TC_CASE(wide_variant)
{
    unsigned char b[64]; _locale_t l = _create_locale(LC_ALL, "C"); FILE *f; int r, n; volatile int i = 5;
    CHECK(l != NULL); if (!l) return;
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) { _free_locale(l); return; }
    r = pf_fwprintf_l(f, L"%d|%ls", l, (int)i, L"w");
    fclose(f);
    CHECK_EQ_I(r, 3);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 3); CHECK_MEM(b, "5|w", 3);
    _free_locale(l); remove(FN);
}
TC_CASE(va_list_variant)
{
    unsigned char b[64]; _locale_t l = _create_locale(LC_ALL, "C"); FILE *f; int r, n; volatile double d = 1.5; volatile int i = 5;
    CHECK(l != NULL); if (!l) return;
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) { _free_locale(l); return; }
    r = vwrap(f, "%d %.2f", l, (int)i, (double)d);
    fclose(f);
    CHECK_EQ_I(r, 6);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 6); CHECK_MEM(b, "5 1.50", 6);
    _free_locale(l); remove(FN);
}
int main(void)
{
    tc_begin(); remove(FN);
    TC_RUN(explicit_c_locale); TC_RUN(null_locale); TC_RUN(wide_variant); TC_RUN(va_list_variant);
    return tc_end();
}
