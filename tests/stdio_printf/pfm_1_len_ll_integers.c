/* TC
 * id:    pfm_1_len_ll_integers
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=80
 * tier:  p0
 * conf:  standard
 * ref:   C99 7.19.6.1p7 (ll); MS docs printf Size prefixes (ll)
 * note:  msvcrt.dll (60) is covered by pfm_1_len_ll_msvcrt_probe.
 */
#include "tc.h"
#include <stdio.h>

static char b[64];
TC_CASE(lli) { volatile long long v = -5000000000LL; sprintf(b, "%lli", v); CHECK_STR(b, "-5000000000"); }
TC_CASE(llo) { volatile unsigned long long v = 8ULL; sprintf(b, "%llo", v); CHECK_STR(b, "10"); }
TC_CASE(llX) { volatile unsigned long long v = 0xCAFEBABEDEADULL; sprintf(b, "%llX", v); CHECK_STR(b, "CAFEBABEDEAD"); }
TC_CASE(lld_min) { volatile long long v = (-9223372036854775807LL - 1); sprintf(b, "%lld", v); CHECK_STR(b, "-9223372036854775808"); }
TC_CASE(llu_max) { volatile unsigned long long v = 18446744073709551615ULL; sprintf(b, "%llu", v); CHECK_STR(b, "18446744073709551615"); }

int main(void)
{
    tc_begin();
    TC_RUN(lli); TC_RUN(llo); TC_RUN(llX); TC_RUN(lld_min); TC_RUN(llu_max);
    return tc_end();
}
