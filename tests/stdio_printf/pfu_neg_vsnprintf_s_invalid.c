/* TC
 * id:    pfu_neg_vsnprintf_s_invalid
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsnprintf_s _vsnwprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _vsnprintf_s: NULL buffer/format -> EINVAL; buffer too small (count>=size, text does not fit) -> handler, buffer[0]=0, -1
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <string.h>

static int cv(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int cw(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnwprintf_s(b, n, c, f, ap); va_end(ap); return r; }

TC_CASE(null_buffer)
{
    char *volatile p = NULL; volatile size_t n = 8, c = 4; int r;
    tc_iph_count = 0; errno = 0;
    r = cv(p, n, c, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[8] = "Q"; const char *volatile f = NULL; volatile size_t n = 8, c = 4; int r;
    tc_iph_count = 0; errno = 0;
    r = cv(b, n, c, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(too_small)
{
    char b[16]; volatile size_t n = 4, c = 10; int r; memset(b, 'Q', sizeof b);
    tc_iph_count = 0;
    r = cv(b, n, c, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(wide_null_buffer)
{
    wchar_t *volatile p = NULL; volatile size_t n = 8, c = 4; int r;
    tc_iph_count = 0; errno = 0;
    r = cw(p, n, c, L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_null_format)
{
    wchar_t w[8]; const wchar_t *volatile f = NULL; volatile size_t n = 8, c = 4; int r;
    w[0] = L'Q';
    tc_iph_count = 0; errno = 0;
    r = cw(w, n, c, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_too_small)
{
    wchar_t w[16]; volatile size_t n = 4, c = 10; int r, i; for (i = 0; i < 16; i++) w[i] = L'Q';
    tc_iph_count = 0;
    r = cw(w, n, c, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(w[0], 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(too_small); 
    TC_RUN(wide_null_buffer); TC_RUN(wide_null_format); TC_RUN(wide_too_small); 
    return tc_end();
}
