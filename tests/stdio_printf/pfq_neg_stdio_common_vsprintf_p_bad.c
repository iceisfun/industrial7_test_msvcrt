/* TC
 * id:    pfq_neg_stdio_common_vsprintf_p_bad
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf_p __stdio_common_vswprintf_p __stdio_common_vfprintf_p
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   UCRT inline wrappers _vsprintf_p / _vswprintf_p / _vfprintf_p (corecrt_stdio_config / stdio.h); MS docs _sprintf_p: NULL -> invalid parameter, returns -1
 * note:  Direct calls with options 0. Observed on Windows 10 22H2: NULL format/buffer/stream -> -1, EINVAL, handler called; a too-small buffer returns a
 *        negative value (-2) without touching bytes beyond count. In the wide worker with options 0 (no legacy-wide-specifier bit) %s means a NARROW
 *        string, so the wide case uses %ls. Unknown options bits are in pfq_neg_stdio_common_vsprintf_p_optbits_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

#define G 0x5A
static int cs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_p(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int cw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf_p(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int cf(FILE *fp, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vfprintf_p(0, fp, f, NULL, ap); va_end(ap); return r; }

TC_CASE(null_format)
{
    char b[16]; const char * volatile f = NULL; const wchar_t * volatile wf = NULL; wchar_t w[16]; int r;
    tc_iph_count = 0; errno = 0;
    r = cs(b, sizeof b, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    tc_iph_count = 0; errno = 0;
    r = cw(w, 16, wf);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(null_buffer)
{
    char * volatile b = NULL; wchar_t * volatile w = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = cs(b, 16, "%1$d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    tc_iph_count = 0; errno = 0;
    r = cw(w, 16, L"%1$d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(null_file)
{
    FILE * volatile fp = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = cf(fp, "%1$d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(too_small)
{
    char b[16]; wchar_t w[16]; volatile size_t n = 4; int r, i, ok = 1;
    for (i = 0; i < 16; i++) { b[i] = G; w[i] = G; }
    r = cs(b, n, "%1$s", "abcdefgh");
    CHECK(r < 0);
    for (i = 4; i < 16; i++) if ((unsigned char)b[i] != G) ok = 0;
    CHECK(ok);
    r = cw(w, n, L"%1$ls", L"abcdefgh");
    CHECK(r < 0);
    ok = 1;
    for (i = 4; i < 16; i++) if (w[i] != G) ok = 0;
    CHECK(ok);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_format); TC_RUN(null_buffer); TC_RUN(null_file); TC_RUN(too_small);
    return tc_end();
}
