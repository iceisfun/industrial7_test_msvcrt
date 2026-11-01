/* TC
 * id:    pff_neg_star_arg_extremes_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: snprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   C11 7.21.6.1 p5 (negative * width = '-' flag with positive width)
 * note:  OBSERVED (Win10 22H2 x64 (19045) ucrtbase.dll observed, zzp_probe2: "star int_min %*f: r=-2147483648 errno=0 iph=0"): snprintf with a width taken from an int
 *        argument equal to INT_MIN (negation not representable) does not crash, does not call the invalid-parameter handler and returns
 *        INT_MIN itself (the computed padded length wraps). The old guess (8 or -1) was wrong. The C standard does not define this
 *        (negative width = - flag + abs; abs(INT_MIN) overflows -> UB) so this is implementation behaviour, kept as a probe-grade
 *        observation. Asserted: no write past the 64-byte count (canary) and r == INT_MIN. Stable (no env dependence).
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

struct tb { char b[64]; char canary[16]; };
static void arm(struct tb *t) { int i; for (i = 0; i < 64; i++) t->b[i] = 'Q'; for (i = 0; i < 16; i++) t->canary[i] = (char)0x5a; }
static int canary_ok(const struct tb *t) { int i; for (i = 0; i < 16; i++) if (t->canary[i] != (char)0x5a) return 0; return 1; }

TC_CASE(width_int_min)
{
    struct tb t; volatile int w = INT_MIN; int r; arm(&t);
    r = snprintf(t.b, 64, "%*f", w, 1.0);
    CHECK(canary_ok(&t)); CHECK_EQ_I(r, INT_MIN);
}
TC_CASE(width_int_min_leftflag)
{
    struct tb t; volatile int w = INT_MIN; int r; arm(&t);
    r = snprintf(t.b, 64, "%-*.2f", w, 1.0);
    CHECK(canary_ok(&t)); CHECK_EQ_I(r, INT_MIN);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(width_int_min); TC_RUN(width_int_min_leftflag); return tc_end(); }
