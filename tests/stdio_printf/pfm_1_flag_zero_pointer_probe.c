/* TC
 * id:    pfm_1_flag_zero_pointer_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf format specification (p: address formatted as hex digits, zero padded to pointer width)
 * note:  PROBES that %p prints sizeof(void*)*2 zero-padded uppercase hex digits and that a 0 flag / narrower width
 *        does not change that, and that %-20p left-aligns that text in a 20 wide field.
 */
#include "tc.h"
#include <stdio.h>

#if TC_X64
#define P1234 "0000000000001234"
#else
#define P1234 "00001234"
#endif
static void * volatile vp = (void *)(ULONG_PTR)0x1234;

TC_CASE(zero16) { char b[64]; sprintf(b, "%016p", vp); CHECK_STR(b, P1234 ); }
TC_CASE(zero_no_width) { char b[64]; sprintf(b, "%0p", vp); CHECK_STR(b, P1234); }
TC_CASE(left20) { char b[64]; int n = sprintf(b, "%-20p", vp); CHECK_EQ_I(n, 20); CHECK(b[0] == P1234[0]); b[sizeof(P1234) - 1] = 0; CHECK_STR(b, P1234); }
TC_CASE(left20_pad) { char b[64]; sprintf(b, "[%-20p]", vp); CHECK(b[21] == ']'); CHECK(b[20] == ' '); CHECK(b[sizeof(P1234)] == ' '); }
int main(void) { tc_begin(); TC_RUN(zero16); TC_RUN(zero_no_width); TC_RUN(left20); TC_RUN(left20_pad); return tc_end(); }
