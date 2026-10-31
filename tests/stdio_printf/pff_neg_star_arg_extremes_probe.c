/* TC
 * id:    pff_neg_star_arg_extremes_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: snprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.1 p5 (negative * width = '-' flag with positive width)
 * note:  PROBES: width taken from an int argument equal to INT_MIN, whose negation is not representable. The standard does
 *        not define this; the guess is that the CRT does not crash, writes nothing past the 64-byte count and either
 *        prints "1.000000" (width ignored) with return 8 or fails with -1. Asserted: no overrun, and r is 8 or -1; if 8,
 *        the text is "1.000000". snprintf (bounded) is used so a huge width cannot overflow the buffer.
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
    CHECK(canary_ok(&t)); CHECK(r == 8 || r == -1);
    if (r == 8) CHECK_STR(t.b, "1.000000");
}
TC_CASE(width_int_min_leftflag)
{
    struct tb t; volatile int w = INT_MIN; int r; arm(&t);
    r = snprintf(t.b, 64, "%-*.2f", w, 1.0);
    CHECK(canary_ok(&t)); CHECK(r == 4 || r == -1);
    if (r == 4) CHECK_STR(t.b, "1.00");
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(width_int_min); TC_RUN(width_int_min_leftflag); return tc_end(); }
