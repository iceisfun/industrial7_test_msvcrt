/* TC
 * id:    str_neg_strncpy_overlap_forward_probe
 * area:  string
 * funcs: strncpy strcpy
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.24.2.3p2 (overlap is undefined)
 * note:  PROBE (informational): short strcpy/strncpy with dest = src - 1 (leftward overlap) yields the same result as memmove on Windows.
 *        Overlap is UB per ISO C and MS docs, so a mismatch is data about the implementation, not a CRT defect.
 */
#include "tc.h"
#include <string.h>

static char *volatile vb;

TC_CASE(strcpy_shift_left)
{
    char b[16]; strcpy(b, "abcdef"); vb = b;
    strcpy(vb, vb + 1);
    CHECK_STR(b, "bcdef");
}
TC_CASE(strncpy_shift_left)
{
    char b[16]; strcpy(b, "abcdef"); vb = b;
    strncpy(vb, vb + 1, 6);
    CHECK_STR(b, "bcdef");
}
int main(void)
{
    tc_begin();
    TC_RUN(strcpy_shift_left); TC_RUN(strncpy_shift_left);
    return tc_end();
}
