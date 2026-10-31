/* TC
 * id:    pfu_snprintf_s_truncate_const
 * area:  stdio_printf
 * funcs: _snprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s: if count is _TRUNCATE, writes as much as fits, NUL-terminates and returns -1 on truncation; no invalid-parameter call
 * note:  errno not asserted here (see pfu_neg_sprintf_s_erange_probe).
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>

TC_CASE(truncates)
{
    char b[16]; volatile size_t c = _TRUNCATE; volatile const char *s = "abcdefghij"; int n, i;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_count = 0;
    n = _snprintf_s(b, 8, c, "%s", (const char *)s);
    CHECK_EQ_I(n, -1); CHECK_STR(b, "abcdefg"); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(b[8], 'Z');
}
TC_CASE(exact_fit)
{
    char b[8]; volatile size_t c = _TRUNCATE; int n; tc_iph_count = 0;
    n = _snprintf_s(b, 8, c, "%s", "abcdefg");
    CHECK_EQ_I(n, 7); CHECK_STR(b, "abcdefg"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(size_one)
{
    char b[1]; volatile size_t c = _TRUNCATE; int n; b[0] = 'x'; tc_iph_count = 0;
    n = _snprintf_s(b, 1, c, "%s", "x");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    tc_iph_install();  /* msvcrt.dll has no handler API; counters then stay 0 */
    TC_RUN(truncates); TC_RUN(exact_fit); TC_RUN(size_one);
    return tc_end();
}
