/* TC
 * id:    pfu_neg_invalid_format_char
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s swprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, swprintf_s: invalid formatting characters invoke the invalid parameter handler, return -1, errno EINVAL
 * note:  Other malformed forms (lone trailing '%', %hhhd, '%!') are in pfu_neg_invalid_format_forms_probe. Output buffer contents after failure are not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(percent_y)
{
    char b[32]; volatile const char *f = "%y"; int r;
    tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, 32, (const char *)f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(percent_y_after_text)
{
    char b[32]; volatile const char *f = "ab%dcd%ye"; int r;
    tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, 32, (const char *)f, 5);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(wide_percent_y)
{
    wchar_t b[32]; volatile const wchar_t *f = L"%y"; int r;
    tc_iph_count = 0; errno = 0;
    r = swprintf_s(b, 32, (const wchar_t *)f);
    CHECK_EQ_I(r, -1); CHECK(tc_iph_count >= 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(valid_still_works)
{
    char b[32]; volatile const char *f = "%d%%"; int r;
    tc_iph_count = 0;
    r = sprintf_s(b, 32, (const char *)f, 9);
    CHECK_EQ_I(r, 2); CHECK_STR(b, "9%"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(percent_y); TC_RUN(percent_y_after_text); TC_RUN(wide_percent_y); TC_RUN(valid_still_works);
    return tc_end();
}
