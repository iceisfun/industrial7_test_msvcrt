/* TC
 * id:    pfu_neg_probe_snprintf_s_size_wrap
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf_s (sizeOfBuffer is size_t)
 * note:  PROBE, isolated from the 0x80000000 case: sizeOfBuffer = SIZE_MAX-1 with count _TRUNCATE and a short output into a
 *        small real buffer. buffer+sizeOfBuffer wraps the address space, so the CRT's internal end-of-buffer arithmetic may
 *        misbehave; the likely (unverified) outcome is plain success with n == 5 and no handler call. Pure guess about
 *        implementation robustness, not a documented guarantee.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(snprintf_s_size_max_minus_2_count_truncate)
{
    char b[16]; volatile size_t sz = (size_t)-2; int n;
    tc_iph_count = 0;
    n = _snprintf_s(b, sz, _TRUNCATE, "abc%d", 12);
    CHECK_EQ_I(n, 5); CHECK_STR(b, "abc12"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(snprintf_s_size_max_minus_2_count_truncate);
    return tc_end();
}
