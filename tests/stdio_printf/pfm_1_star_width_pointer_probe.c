/* TC
 * id:    pfm_1_star_width_pointer_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf format specification (p)
 * note:  PROBES that %*p with width 20 yields a 20 character right-aligned field holding sizeof(void*)*2 uppercase hex digits.
 */
#include "tc.h"
#include <stdio.h>

#if TC_X64
#define P1234 "0000000000001234"
#else
#define P1234 "00001234"
#endif
static void * volatile vp = (void *)(ULONG_PTR)0x1234;

TC_CASE(right20) { char b[64]; int n = sprintf(b, "%*p", 20, vp); CHECK_EQ_I(n, 20); CHECK_STR(b + 20 - (sizeof(P1234) - 1), P1234); CHECK(b[0] == ' '); }
TC_CASE(left20) { char b[64]; int n = sprintf(b, "%-*p", 20, vp); CHECK_EQ_I(n, 20); CHECK(b[19] == ' '); b[sizeof(P1234) - 1] = 0; CHECK_STR(b, P1234); }
int main(void) { tc_begin(); TC_RUN(right20); TC_RUN(left20); return tc_end(); }
