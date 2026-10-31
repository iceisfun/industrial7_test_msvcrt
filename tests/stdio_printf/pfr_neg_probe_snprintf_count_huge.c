/* TC
 * id:    pfr_neg_probe_snprintf_count_huge
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf _snwprintf
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.5 (snprintf n is a size_t); MS docs _snprintf (count = maximum characters to write)
 * note:  PROBE: count == SIZE_MAX (and > INT_MAX) with a short output and a small valid buffer. Expected most likely: accepted,
 *        text written, length returned (count only bounds the output). Real Windows might instead reject huge counts
 *        (-1 / EINVAL / ERANGE) - that is what is measured. Each function and count value is its own case; the 0x80000000
 *        cases only assert "no crash, and either full success or -1 with an untouched-or-terminated buffer".
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(snprintf_legacy_max)
{
    char b[16]; int r;
    b[0] = 0; tc_iph_count = 0; errno = 0;
    r = _snprintf(b, (size_t)-1, "%s", "abc");
    CHECK_EQ_I(r, 3); CHECK_STR(b, "abc");
}
TC_CASE(snwprintf_max)
{
    wchar_t b[16]; int r;
    b[0] = 0; tc_iph_count = 0; errno = 0;
    r = _snwprintf(b, (size_t)-1, L"%ls", L"abc");
    CHECK_EQ_I(r, 3); CHECK_WSTR(b, L"abc");
}
#if TC_CRT_VER >= 140
TC_CASE(snprintf_c99_max)
{
    char b[16]; int r;
    b[0] = 0; tc_iph_count = 0; errno = 0;
    r = snprintf(b, (size_t)-1, "%s", "abc");
    CHECK_EQ_I(r, 3); CHECK_STR(b, "abc");
}
#endif
TC_CASE(legacy_2gb_loose)
{
    char b[16]; int r; volatile size_t n = (size_t)0x80000000u;
    b[0] = 0;
    r = _snprintf(b, n, "%s", "abc");
    CHECK(r == 3 || r == -1);
    if (r == 3) CHECK_STR(b, "abc");
}
TC_CASE(wide_2gb_loose)
{
    wchar_t b[16]; int r; volatile size_t n = (size_t)0x80000000u;
    b[0] = 0;
    r = _snwprintf(b, n, L"%ls", L"abc");
    CHECK(r == 3 || r == -1);
    if (r == 3) CHECK_WSTR(b, L"abc");
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(snprintf_legacy_max); TC_RUN(snwprintf_max);
#if TC_CRT_VER >= 140
    TC_RUN(snprintf_c99_max);
#endif
    TC_RUN(legacy_2gb_loose); TC_RUN(wide_2gb_loose);
    return tc_end();
}
