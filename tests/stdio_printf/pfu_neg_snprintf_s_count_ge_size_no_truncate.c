/* TC
 * id:    pfu_neg_snprintf_s_count_ge_size_no_truncate
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s: if sizeOfBuffer is too small for the output and count is not _TRUNCATE, invalid parameter handler; returns -1
 * note:  errno is covered by pfu_neg_sprintf_s_erange_probe; buffer[0]==0 after failure is probed in pfu_neg_sprintf_s_overflow_buf_probe. Bytes at index >= sizeOfBuffer must stay untouched.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(overflow)
{
    char b[16]; volatile size_t sz = 4, c = 10; int n, i;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_count = 0;
    n = _snprintf_s(b, sz, c, "%s", "abcdef");
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    for (i = 4; i < 16; i++) CHECK_EQ_I(b[i], 'Z');
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    TC_RUN(overflow);
    return tc_end();
}
