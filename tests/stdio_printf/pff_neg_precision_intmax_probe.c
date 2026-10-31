/* TC
 * id:    pff_neg_precision_intmax_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: snprintf sprintf_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs sprintf_s (buffer too small -> -1, ERANGE, buf[0]=0); C11 7.21.6.5 (snprintf writes at most n-1 chars)
 * note:  PROBES the extreme-but-representable case: precision INT_MAX / width INT_MAX in a 32-byte buffer. The CRT either
 *        fails (-1, e.g. cannot allocate the conversion buffer) or produces a huge would-be length; it must never write past
 *        the buffer. Asserted: no overrun (canary), sprintf_s returns -1 with buf[0]==0. The snprintf return value is only
 *        asserted to be -1 or >= 32 (truncated). Run on real Windows to learn which; may be slow if it really formats 2 GiB.
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

struct tb { char b[32]; char canary[16]; };
static void arm(struct tb *t) { int i; for (i = 0; i < 32; i++) t->b[i] = 'Q'; for (i = 0; i < 16; i++) t->canary[i] = (char)0x5a; }
static int canary_ok(const struct tb *t) { int i; for (i = 0; i < 16; i++) if (t->canary[i] != (char)0x5a) return 0; return 1; }

TC_CASE(sprintf_s_prec_intmax)
{
    struct tb t; const char * volatile f = "%.2147483647f"; int r; arm(&t);
    r = sprintf_s(t.b, 32, f, 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(t.b[0], 0); CHECK(canary_ok(&t));
}
TC_CASE(snprintf_star_prec_intmax)
{
    struct tb t; volatile int p = INT_MAX; int r; arm(&t);
    r = snprintf(t.b, 32, "%.*f", p, 1.0);
    CHECK(r == -1 || r >= 32); CHECK(canary_ok(&t));
}
TC_CASE(snprintf_star_width_intmax)
{
    struct tb t; volatile int w = INT_MAX; int r; arm(&t);
    r = snprintf(t.b, 32, "%*.*f", w, 2, 1.0);
    CHECK(r == -1 || r >= 32); CHECK(canary_ok(&t));
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_s_prec_intmax); TC_RUN(snprintf_star_prec_intmax); TC_RUN(snprintf_star_width_intmax);
    return tc_end();
}
