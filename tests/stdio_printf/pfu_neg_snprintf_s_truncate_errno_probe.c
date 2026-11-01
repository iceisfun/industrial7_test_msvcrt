/* TC
 * id:    pfu_neg_snprintf_s_truncate_errno_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s _snwprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _snprintf_s (return value, errno)
 * note:  errno after _TRUNCATE truncation. Observed Win10 22H2 ucrtbase (corrects the old ERANGE guess): errno stays 0 on _TRUNCATE truncation, narrow and wide; count-limited truncation (count<size) also leaves errno 0. Return -1 and no handler are also asserted (documented).
 *        Raw evidence (baseline): truncate_errno errno == ERANGE got=0 want=34 ; wide_truncate_errno got=0.
 *        crt narrowed to ucrtbase: the observed behaviour was measured on Win10 22H2 ucrtbase only (older CRTs [unverified]).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif

TC_CASE(truncate_errno)
{
    char b[16]; volatile size_t n = 4, c = _TRUNCATE; int r;
    tc_iph_count = 0; errno = 0;
    r = _snprintf_s(b, n, c, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "abc");
    CHECK_EQ_I(errno, 0);   /* observed: not set */
}
TC_CASE(count_errno)
{
    char b[16]; volatile size_t n = 16, c = 3; int r;
    tc_iph_count = 0; errno = 0;
    r = _snprintf_s(b, n, c, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "abc");
    CHECK_EQ_I(errno, 0);
}
TC_CASE(wide_truncate_errno)
{
    wchar_t b[16]; volatile size_t n = 4, c = _TRUNCATE; int r;
    tc_iph_count = 0; errno = 0;
    r = _snwprintf_s(b, n, c, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK_WSTR(b, L"abc");
    CHECK_EQ_I(errno, 0);   /* observed: not set */
}
TC_CASE(wide_count_errno)
{
    wchar_t b[16]; volatile size_t n = 16, c = 3; int r;
    tc_iph_count = 0; errno = 0;
    r = _snwprintf_s(b, n, c, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK_WSTR(b, L"abc");
    CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(truncate_errno); TC_RUN(count_errno); TC_RUN(wide_truncate_errno); TC_RUN(wide_count_errno);
    return tc_end();
}
