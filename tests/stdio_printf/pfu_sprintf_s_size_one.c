/* TC
 * id:    pfu_sprintf_s_size_one
 * area:  stdio_printf
 * funcs: sprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s: returns number of characters written, not counting the terminating NUL; the NUL needs one element
 */
#include "tc.h"
#include <stdio.h>

static char fmt_e[] = "";
static char fmt_s[] = "%s";

TC_CASE(size_one_empty_ok)
{
    char b[8] = "QQQ"; volatile size_t n = 1; int r;
    r = sprintf_s(b, n, fmt_e);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(b[0], 0);
}
TC_CASE(size_two_one_char)
{
    char b[8] = "QQQ"; volatile size_t n = 2; int r;
    r = sprintf_s(b, n, fmt_s, "a");
    CHECK_EQ_I(r, 1); CHECK_STR(b, "a");
}
int main(void)
{
    tc_begin();
    TC_RUN(size_one_empty_ok); TC_RUN(size_two_one_char);
    return tc_end();
}
