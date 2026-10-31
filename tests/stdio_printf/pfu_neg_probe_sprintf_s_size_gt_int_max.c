/* TC
 * id:    pfu_neg_probe_sprintf_s_size_gt_int_max
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s swprintf_s
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs sprintf_s (sizeOfBuffer is size_t)
 * note:  PROBE: sizeOfBuffer is only an upper bound. With a huge sizeOfBuffer (0x80000000) and a short output
 *        into a small real buffer the calls are expected to succeed and return the length (no truncation, no handler call).
 *        Assumes release ucrtbase does not fill the whole claimed buffer.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(sprintf_s_0x80000000)
{
    char b[16]; volatile size_t sz = (size_t)0x80000000u; int n;
    tc_iph_count = 0;
    n = sprintf_s(b, sz, "abc");
    CHECK_EQ_I(n, 3); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(swprintf_s_big_size)
{
    wchar_t b[16]; volatile size_t sz = (size_t)0x80000000u; int n;
    tc_iph_count = 0;
    n = swprintf_s(b, sz, L"abc");
    CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(sprintf_s_0x80000000); TC_RUN(swprintf_s_big_size);
    return tc_end();
}
