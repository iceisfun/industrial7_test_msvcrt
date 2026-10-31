/* TC
 * id:    gcv_swab_basic
 * area:  conv
 * funcs: _swab
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _swab
 * note:  Even counts only; odd-count behaviour is in gcv_swab_odd_probe.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(six_bytes)
{
    static char src[] = "ABCDEF"; char dst[8] = {0};
    _swab(src, dst, 6);
    CHECK_MEM(dst, "BADCFE", 6);
    CHECK_STR(src, "ABCDEF");
}
TC_CASE(single_pair)
{
    static char src[] = "xy"; char dst[4] = {'.','.','.','.'};
    _swab(src, dst, 2);
    CHECK_MEM(dst, "yx..", 4);
}
TC_CASE(zero_untouched)
{
    static char src[] = "ab"; char dst[3] = {'q','r','s'};
    volatile int n = 0;
    _swab(src, dst, n);
    CHECK_MEM(dst, "qrs", 3);
}
TC_CASE(binary)
{
    static unsigned char src[6] = {0x00, 0xFF, 0x01, 0x00, 0x80, 0x7F};
    unsigned char dst[6] = {9,9,9,9,9,9};
    static const unsigned char want[6] = {0xFF, 0x00, 0x00, 0x01, 0x7F, 0x80};
    _swab((char *)src, (char *)dst, 6);
    CHECK_MEM(dst, want, 6);
    CHECK_EQ_I(src[0], 0x00); CHECK_EQ_I(src[1], 0xFF); CHECK_EQ_I(src[5], 0x7F);
}
int main(void)
{
    tc_begin();
    TC_RUN(six_bytes); TC_RUN(single_pair); TC_RUN(zero_untouched); TC_RUN(binary);
    return tc_end();
}
