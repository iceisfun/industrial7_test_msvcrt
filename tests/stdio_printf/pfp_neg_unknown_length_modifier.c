/* TC
 * id:    pfp_neg_unknown_length_modifier
 * area:  stdio_printf
 * funcs: sprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf size prefixes (hh h l ll j z t I I32 I64 w L)
 * note:  PROBE: sprintf_s rejects invalid size-prefix combos are rejected by the ucrt format state machine. %Z omitted (valid type char).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

/* A bad format must fail: return < 0, errno EINVAL, invalid-parameter handler called. */
static void bad(const char *fmt)
{
    char b[32]; int n, i;
    for (i = 0; i < 32; i++) b[i] = 'X';
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    n = sprintf_s(b, sizeof b, fmt, 1, 2);
    CHECK(n < 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
static const char *volatile F1 = "%hhhd", *volatile F2 = "%lll d", *volatile F3 = "%llllx", *volatile F4 = "%qd", *volatile F5 = "%lhd";
TC_CASE(hhh)  { bad(F1); }
TC_CASE(lll_space) { bad(F2); }
TC_CASE(llll) { bad(F3); }
TC_CASE(q)    { bad(F4); }
TC_CASE(lh)   { bad(F5); }

int main(void) { tc_begin(); TC_RUN(hhh); TC_RUN(lll_space); TC_RUN(llll); TC_RUN(q); TC_RUN(lh); return tc_end(); }
