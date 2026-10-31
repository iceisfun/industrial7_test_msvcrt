/* TC
 * id:    pfq_stdio_common_vfprintf_p_direct
 * area:  stdio_printf
 * funcs: __stdio_common_vfprintf_p __stdio_common_vfwprintf_p
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   <stdio.h> in the Windows 10 SDK (inline _vfprintf_p_l / _vfwprintf_p_l); undocumented internal API
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#define FN "tc_pfq_stdio_common_vfprintf_p_direct.tmp"

static int cf(FILE *s, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vfprintf_p(0, s, f, NULL, ap); va_end(ap); return r; }
static int cfw(FILE *s, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vfwprintf_p(0, s, f, NULL, ap); va_end(ap); return r; }
static size_t slurp(char *buf, size_t cap)
{
    FILE *f = fopen(FN, "rb"); size_t n = 0;
    if (f) { n = fread(buf, 1, cap - 1, f); fclose(f); }
    buf[n] = 0; return n;
}

TC_CASE(narrow)
{
    char b[64]; FILE *f = fopen(FN, "wb"); int r;
    CHECK(f != NULL); if (!f) return;
    r = cf(f, "%2$s %1$d", 5, "x");
    fclose(f);
    CHECK_EQ_I(r, 3);
    CHECK_EQ_U(slurp(b, sizeof b), 3);
    CHECK_STR(b, "x 5");
}
TC_CASE(reuse)
{
    char b[64]; FILE *f = fopen(FN, "wb"); int r;
    CHECK(f != NULL); if (!f) return;
    r = cf(f, "%1$d/%1$d/%2$d", 4, 9);
    fclose(f);
    CHECK_EQ_I(r, 5);
    slurp(b, sizeof b);
    CHECK_STR(b, "4/4/9");
}
TC_CASE(wide_to_ansi_file)
{
    char b[64]; FILE *f = fopen(FN, "w"); int r;
    CHECK(f != NULL); if (!f) return;
    r = cfw(f, L"%2$ls=%1$d", 3, L"abc");
    fclose(f);
    CHECK_EQ_I(r, 5);
    slurp(b, sizeof b);
    CHECK_STR(b, "abc=3");
}
TC_CASE(cleanup) { CHECK_EQ_I(remove(FN), 0); }
int main(void)
{
    tc_begin();
    TC_RUN(narrow); TC_RUN(reuse); TC_RUN(wide_to_ansi_file); TC_RUN(cleanup);
    return tc_end();
}
