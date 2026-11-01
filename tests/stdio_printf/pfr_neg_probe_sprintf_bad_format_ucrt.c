/* TC
 * id:    pfr_neg_probe_sprintf_bad_format_ucrt
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf _snprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf format specification; UCRT parameter validation
 * note:  Observed Win10 22H2 ucrtbase: the plain (non-_s) family does NOT reject malformed specs: output is the literal text, errno 0, handler not called. Cases kept separate:
 *        trailing lone '%', "%z" (size prefix then end of string), "%Q" (unknown type char) via _snprintf.
 *        Raw evidence (baseline): trailing_percent r == -1 got=3 ; percent_z got=0 ; snprintf_Q got=1 ; errno == EINVAL got=0 and tc_iph_count == 1 got=0 in all three.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(trailing_percent)
{
    char b[32]; const char * volatile f = "abc%"; int r;
    tc_iph_count = 0; errno = 0;
    r = sprintf(b, f);
    CHECK_EQ_I(r, 3); CHECK_STR(b, "abc"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(percent_z)
{
    char b[32]; const char * volatile f = "%z"; int r;
    tc_iph_count = 0; errno = 0;
    r = sprintf(b, f);
    CHECK_EQ_I(r, 0); CHECK_STR(b, ""); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(snprintf_Q)
{
    char b[32]; const char * volatile f = "%Q"; int r;
    tc_iph_count = 0; errno = 0;
    r = _snprintf(b, 16, f);
    CHECK_EQ_I(r, 1); CHECK_STR(b, "Q"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(trailing_percent); TC_RUN(percent_z); TC_RUN(snprintf_Q); return tc_end();
}
