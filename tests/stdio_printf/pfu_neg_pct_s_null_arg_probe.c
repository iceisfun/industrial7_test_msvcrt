/* TC
 * id:    pfu_neg_pct_s_null_arg_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s swprintf_s _snprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf format specification (%s)
 * note:  PROBING exactly: whether the secure variants print "(null)" for a NULL %s / %ls argument, as the non-secure printf family does, or invoke the invalid parameter handler. The docs do not say; expectation is "(null)" with return 6 and no handler. The precision-0 case expects an empty string (the substituted text is truncated to 0 chars).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(sprintf_s_null)
{
    char b[32]; const char *volatile p = NULL; int r;
    tc_iph_count = 0;
    r = sprintf_s(b, 32, "%s", p);
    CHECK_EQ_I(r, 6); CHECK_STR(b, "(null)"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(snprintf_s_null)
{
    char b[32]; const char *volatile p = NULL; int r;
    tc_iph_count = 0;
    r = _snprintf_s(b, 32, 31, "[%s]", p);
    CHECK_EQ_I(r, 8); CHECK_STR(b, "[(null)]"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(swprintf_s_null)
{
    wchar_t b[32]; const wchar_t *volatile p = NULL; int r;
    tc_iph_count = 0;
    r = swprintf_s(b, 32, L"%ls", p);
    CHECK_EQ_I(r, 6); CHECK_WSTR(b, L"(null)"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_precision_zero)
{
    char b[32]; const char *volatile p = NULL; int r;
    tc_iph_count = 0;
    r = sprintf_s(b, 32, "[%.0s]", p);
    CHECK_EQ_I(r, 2); CHECK_STR(b, "[]"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(sprintf_s_null); TC_RUN(snprintf_s_null); TC_RUN(swprintf_s_null); TC_RUN(null_precision_zero);
    return tc_end();
}
