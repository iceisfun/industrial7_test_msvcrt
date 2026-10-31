/* TC
 * id:    pfm_1_len_ll_msvcrt_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf Size prefixes (ll)
 * note:  PROBES whether the Windows 7 msvcrt.dll understands ll (expected yes: "-5000000000").
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(lli) { char b[32]; volatile long long v = -5000000000LL; sprintf(b, "%lli", v); CHECK_STR(b, "-5000000000"); }
TC_CASE(llX) { char b[32]; volatile unsigned long long v = 0xCAFEBABEDEADULL; sprintf(b, "%llX", v); CHECK_STR(b, "CAFEBABEDEAD"); }

int main(void) { tc_begin(); TC_RUN(lli); TC_RUN(llX); return tc_end(); }
