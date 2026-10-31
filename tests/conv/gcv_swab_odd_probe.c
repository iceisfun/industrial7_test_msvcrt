/* TC
 * id:    gcv_swab_odd_probe
 * area:  conv
 * funcs: _swab
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _swab (n should be even)
 * note:  PROBE: with odd n, the last byte is not copied (n-1 bytes swapped, dest[n-1] untouched). Docs only say n should be even.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(odd_count)
{
    static char src[] = "ABCDE"; char dst[6] = {'.','.','.','.','.','.'};
    volatile int n = 5;
    _swab(src, dst, n);
    CHECK_MEM(dst, "BADC..", 6);
}
int main(void)
{
    tc_begin();
    TC_RUN(odd_count);
    return tc_end();
}
