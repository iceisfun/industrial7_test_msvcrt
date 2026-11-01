/* TC
 * id:    pfu_stdio_common_vsprintf_s_direct_ok
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf_s __stdio_common_vsnprintf_s __stdio_common_vswprintf_s __stdio_common_vsnwprintf_s __stdio_common_vfprintf_s __stdio_common_vfwprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf_s, _snprintf_s, swprintf_s, _snwprintf_s, fprintf_s, fwprintf_s (UCRT inline wrappers call the __stdio_common_v*_s workers with options 0)
 * note:  Direct calls of the UCRT printf workers; success paths only. Worker signatures follow corecrt_stdio_config.h/stdio.h of the Windows SDK: (options,
 *        [stream|buffer, bufcount, [maxcount,]] format, locale, arglist). The wide stream is opened in TEXT mode ("w"): observed on Windows 10 22H2, a
 *        binary-mode ("wb") ANSI stream receives the raw UTF-16 code units (w,0,=,0,o,0,k,0), only text mode converts to multibyte "w=ok".
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#define G 0x5A

static int w_vsprintf_s(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int w_vsnprintf_s(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsnprintf_s(0, b, n, c, f, NULL, ap); va_end(ap); return r; }
static int w_vswprintf_s(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int w_vsnwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsnwprintf_s(0, b, n, c, f, NULL, ap); va_end(ap); return r; }
static int w_vfprintf_s(FILE *fp, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vfprintf_s(0, fp, f, NULL, ap); va_end(ap); return r; }
static int w_vfwprintf_s(FILE *fp, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vfwprintf_s(0, fp, f, NULL, ap); va_end(ap); return r; }

TC_CASE(vsprintf_s_len)
{
    char b[32]; volatile size_t n = 32; int r;
    r = w_vsprintf_s(b, n, "%d-%s", 42, "xyz");
    CHECK_EQ_I(r, 6); CHECK_STR(b, "42-xyz");
}
TC_CASE(vsnprintf_s_count)
{
    char b[16]; volatile size_t n = 16, c = 3; int r, i;
    for (i = 0; i < 16; i++) b[i] = G;
    r = w_vsnprintf_s(b, n, c, "%s", "abcdef");   /* count < len: truncated, -1 (docs) */
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc");
    r = w_vsnprintf_s(b, n, 10, "%s", "abc");     /* fits */
    CHECK_EQ_I(r, 3); CHECK_STR(b, "abc");
}
TC_CASE(vswprintf_s_wide)
{
    wchar_t b[32]; volatile size_t n = 32; int r;
    r = w_vswprintf_s(b, n, L"%d-%ls", 7, L"hi");
    CHECK_EQ_I(r, 4); CHECK_WSTR(b, L"7-hi");
}
TC_CASE(vsnwprintf_s_truncate)
{
    wchar_t b[16]; volatile size_t n = 4; int r;
    r = w_vsnwprintf_s(b, n, (size_t)-1, L"%ls", L"abcdef");   /* _TRUNCATE */
    CHECK_EQ_I(r, -1); CHECK_WSTR(b, L"abc");
}
static void read_back(const char *name, char *b, size_t cap)
{
    FILE *f = fopen(name, "rb"); size_t k = 0;
    if (f) { k = fread(b, 1, cap - 1, f); fclose(f); }
    b[k] = 0; remove(name);
}
TC_CASE(vfprintf_s_file)
{
    FILE *f = fopen("tc_pfu_direct_a.tmp", "wb"); char b[32]; int r;
    CHECK(f != NULL); if (!f) return;
    r = w_vfprintf_s(f, "n=%d;", 5);
    fclose(f);
    read_back("tc_pfu_direct_a.tmp", b, sizeof b);
    CHECK_EQ_I(r, 4); CHECK_STR(b, "n=5;");
}
TC_CASE(vfwprintf_s_file)
{
    FILE *f = fopen("tc_pfu_direct_w.tmp", "w"); char b[32]; int r;
    CHECK(f != NULL); if (!f) return;
    r = w_vfwprintf_s(f, L"w=%ls", L"ok");   /* ANSI-mode stream: wide output is converted to multibyte (docs fwprintf) */
    fclose(f);
    read_back("tc_pfu_direct_w.tmp", b, sizeof b);
    CHECK_EQ_I(r, 4); CHECK_STR(b, "w=ok");
}
int main(void)
{
    tc_begin();
    TC_RUN(vsprintf_s_len); TC_RUN(vsnprintf_s_count); TC_RUN(vswprintf_s_wide);
    TC_RUN(vsnwprintf_s_truncate); TC_RUN(vfprintf_s_file); TC_RUN(vfwprintf_s_file);
    return tc_end();
}
