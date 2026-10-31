/* TC
 * id:    pfu_neg_vsprintf_s_invalid
 * area:  stdio_printf
 * kind:  negative
 * funcs: vsprintf_s vswprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vsprintf_s: NULL buffer/format -> EINVAL; too small -> buffer[0]=0, -1; handler invoked (zero-size cases live in pfu_neg_sprintf_s_zero_size, probe)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <string.h>

static int cv(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vsprintf_s(b, n, f, ap); va_end(ap); return r; }
static int cw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vswprintf_s(b, n, f, ap); va_end(ap); return r; }

TC_CASE(too_small)
{
    char b[16]; volatile size_t n = 4; int r; memset(b, 'Q', sizeof b);
    tc_iph_count = 0;
    r = cv(b, n, "%s", "abcdef");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(null_buffer)
{
    char *volatile p = NULL; volatile size_t n = 8; int r;
    tc_iph_count = 0; errno = 0;
    r = cv(p, n, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[8] = "Q"; const char *volatile f = NULL; volatile size_t n = 8; int r;
    tc_iph_count = 0; errno = 0;
    r = cv(b, n, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_too_small)
{
    wchar_t w[16]; volatile size_t n = 4; int r, i; for (i = 0; i < 16; i++) w[i] = L'Q';
    tc_iph_count = 0;
    r = cw(w, n, L"%ls", L"abcdef");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(w[0], 0);
}
TC_CASE(wide_null_buffer)
{
    wchar_t *volatile p = NULL; volatile size_t n = 8; int r;
    tc_iph_count = 0; errno = 0;
    r = cw(p, n, L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_null_format)
{
    wchar_t w[8]; const wchar_t *volatile f = NULL; volatile size_t n = 8; int r;
    w[0] = L'Q';
    tc_iph_count = 0; errno = 0;
    r = cw(w, n, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(too_small); TC_RUN(null_buffer); TC_RUN(null_format); 
    TC_RUN(wide_too_small); TC_RUN(wide_null_buffer); TC_RUN(wide_null_format); 
    return tc_end();
}
