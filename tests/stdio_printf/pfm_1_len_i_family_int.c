/* TC
 * id:    pfm_1_len_i_family_int
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf Size prefixes: I (ptrdiff_t/size_t/intptr_t by pointer width), I32, I64
 * note:  %In (count) intentionally not exercised. Plain "I" is asserted from VS2005 (msvcr80) up only; msvcrt.dll (60) is covered by pfm_1_len_i_msvcrt_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stddef.h>

static char b[64];
TC_CASE(I32i)    { volatile int v = -5; sprintf(b, "%I32i", v); CHECK_STR(b, "-5"); }
TC_CASE(I32u)    { volatile unsigned v = 0xFFFFFFFFu; sprintf(b, "%I32u", v); CHECK_STR(b, "4294967295"); }
TC_CASE(I64i)    { volatile long long v = -5000000000LL; sprintf(b, "%I64i", v); CHECK_STR(b, "-5000000000"); }
TC_CASE(I64u)    { volatile unsigned long long v = 18446744073709551615ULL; sprintf(b, "%I64u", v); CHECK_STR(b, "18446744073709551615"); }
TC_CASE(I64X)    { volatile unsigned long long v = 0xCAFEBABEDEADULL; sprintf(b, "%I64X", v); CHECK_STR(b, "CAFEBABEDEAD"); }
TC_CASE(Ii)      { volatile intptr_t v = -7; sprintf(b, "%Ii", v); CHECK_STR(b, "-7"); }
TC_CASE(Iu)      { volatile size_t v = 7; sprintf(b, "%Iu", v); CHECK_STR(b, "7"); }

int main(void)
{
    tc_begin();
    TC_RUN(I32i); TC_RUN(I32u); TC_RUN(I64i); TC_RUN(I64u); TC_RUN(I64X);
    TC_RUN(Ii); TC_RUN(Iu);
    return tc_end();
}
