/* TC
 * id:    pfp_neg_swprintf_s_truncate
 * area:  stdio_printf
 * funcs: swprintf_s _snwprintf_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs swprintf_s, _snwprintf_s: overflow/NULL -> handler; _TRUNCATE truncates silently
 * note:  conf observed: relies on %p being 16 hex digits on x64
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <stdarg.h>

TC_CASE(too_small)
{
    wchar_t b[24]; volatile void *p = (void *)(size_t)0xABC; int r, i;
    if (!TC_X64) TC_SKIP("x64 only");
    for (i = 0; i < 24; i++) b[i] = L'Z';
    tc_iph_count = 0;
    r = swprintf_s(b, 16, L"%p", p);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(null_buf)
{
    wchar_t *volatile nb = NULL; int r;
    tc_iph_count = 0;
    r = swprintf_s(nb, 16, L"%d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_fmt)
{
    wchar_t b[16]; const wchar_t *volatile f = NULL; int r;
    tc_iph_count = 0;
    r = swprintf_s(b, 16, f);
    CHECK_EQ_I(r, -1);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(truncate)
{
    wchar_t b[16]; volatile void *p = (void *)(size_t)0xABCDEF; int r, i;
    if (!TC_X64) TC_SKIP("x64 only");
    for (i = 0; i < 16; i++) b[i] = L'Z';
    tc_iph_count = 0;
    r = _snwprintf_s(b, 8, _TRUNCATE, L"%p", p);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(b[7], 0);
    CHECK_WSTR(b, L"0000000");
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(b[8], L'Z');
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(too_small); TC_RUN(null_buf); TC_RUN(null_fmt); TC_RUN(truncate); return tc_end(); }
