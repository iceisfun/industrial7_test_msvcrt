/* TC
 * id:    pfu_neg_snprintf_s_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s _snwprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s: buffer or format NULL -> invalid parameter handler, returns -1, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif

TC_CASE(null_buffer)
{
    char *volatile p = NULL; volatile size_t c = 5; int r;
    tc_iph_count = 0; errno = 0;
    r = _snprintf_s(p, 10, c, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buffer_truncate)
{
    char *volatile p = NULL; volatile size_t c = _TRUNCATE; int r;
    tc_iph_count = 0; errno = 0;
    r = _snprintf_s(p, 10, c, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[10] = "Q"; const char *volatile f = NULL; volatile size_t c = 5; int r;
    tc_iph_count = 0; errno = 0;
    r = _snprintf_s(b, 10, c, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_null_buffer)
{
    wchar_t *volatile p = NULL; volatile size_t c = 5; int r;
    tc_iph_count = 0; errno = 0;
    r = _snwprintf_s(p, 10, c, L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_null_buffer_truncate)
{
    wchar_t *volatile p = NULL; volatile size_t c = _TRUNCATE; int r;
    tc_iph_count = 0; errno = 0;
    r = _snwprintf_s(p, 10, c, L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_null_format)
{
    wchar_t b[10]; const wchar_t *volatile f = NULL; volatile size_t c = 5; int r;
    b[0] = L'Q';
    tc_iph_count = 0; errno = 0;
    r = _snwprintf_s(b, 10, c, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_buffer); TC_RUN(null_buffer_truncate); TC_RUN(null_format);
    TC_RUN(wide_null_buffer); TC_RUN(wide_null_buffer_truncate); TC_RUN(wide_null_format);
    return tc_end();
}
