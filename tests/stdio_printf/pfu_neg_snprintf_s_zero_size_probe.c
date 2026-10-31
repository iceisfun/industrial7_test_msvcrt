/* TC
 * id:    pfu_neg_snprintf_s_zero_size_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s _snwprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _snprintf_s (sizeOfBuffer, count)
 * note:  PROBE: behaviour for sizeOfBuffer==0 (with count 0, count _TRUNCATE, count>0) and for NULL buffer with size 0 count 0. Expectation (most likely): return -1 and invalid-parameter handler called once, buffer untouched, except possibly count==0 which some CRT versions may accept as a no-op returning 0; we assert only -1+handler for count>0/_TRUNCATE, and record count==0 weakly (-1 handler or 0 no handler).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif

TC_CASE(size0_count_truncate)
{
    char b[8] = "QQ"; volatile size_t n = 0, c = _TRUNCATE; int r;
    tc_iph_count = 0;
    r = _snprintf_s(b, n, c, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 'Q');
}
TC_CASE(size0_count_pos)
{
    char b[8] = "QQ"; volatile size_t n = 0, c = 1; int r;
    tc_iph_count = 0;
    r = _snprintf_s(b, n, c, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 'Q');
}
TC_CASE(size0_count0)
{
    char b[8] = "QQ"; volatile size_t n = 0, c = 0; int r;
    tc_iph_count = 0;
    r = _snprintf_s(b, n, c, "x");
    CHECK((r == -1 && tc_iph_count == 1) || (r == 0 && tc_iph_count == 0));
    CHECK_EQ_I(b[0], 'Q');
}
TC_CASE(null_size0_count0)
{
    char *volatile p = NULL; volatile size_t n = 0, c = 0; int r;
    tc_iph_count = 0;
    r = _snprintf_s(p, n, c, "x");
    CHECK((r == -1 && tc_iph_count == 1) || (r == 0 && tc_iph_count == 0));
}
TC_CASE(wide_size0_count_truncate)
{
    wchar_t b[8] = { L'Q', L'Q' }; volatile size_t n = 0, c = _TRUNCATE; int r;
    tc_iph_count = 0;
    r = _snwprintf_s(b, n, c, L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], L'Q');
}
TC_CASE(wide_count0_size_ok)
{
    wchar_t b[8] = { L'Q', L'Q' }; volatile size_t n = 8, c = 0; int r;
    tc_iph_count = 0;
    r = _snwprintf_s(b, n, c, L"abc");
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK(r == 0 || r == -1);
    CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(size0_count_truncate); TC_RUN(size0_count_pos); TC_RUN(size0_count0); TC_RUN(null_size0_count0);
    TC_RUN(wide_size0_count_truncate); TC_RUN(wide_count0_size_ok);
    return tc_end();
}
