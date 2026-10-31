/* TC
 * id:    pfm_2_target_old_crt_i64_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr80 msvcr90 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf Size and distance specifiers (%I64d is a Microsoft extension)
 * note:  PROBES that %I64d / %I64u / %I64x work on msvcr80, msvcr90 and system msvcrt.dll (documented for VS2005+; msvcrt.dll
 *        assumed from its VC6 lineage), including unsigned values above 2^32 and negative values.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(i64_forms)
{
    char b[64]; volatile long long s = -5000000000LL; volatile unsigned long long u = 18446744073709551615ULL, x = 0x123456789abcULL;
    const char * volatile f = "%I64d|%I64u|%I64x|%I64X";
    int n = sprintf(b, f, s, u, x, x);
    CHECK_STR(b, "-5000000000|18446744073709551615|123456789abc|123456789ABC"); CHECK_EQ_I(n, 11 + 1 + 20 + 1 + 12 + 1 + 12);
}
int main(void) { tc_begin(); TC_RUN(i64_forms); return tc_end(); }
