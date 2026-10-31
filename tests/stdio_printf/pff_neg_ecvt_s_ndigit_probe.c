/* TC
 * id:    pff_neg_ecvt_s_ndigit_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _ecvt_s _fcvt_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _ecvt_s, _fcvt_s
 * note:  PROBES: _ecvt_s with ndigit 0 and negative ndigit; _fcvt_s with a negative digit count (rounding left of the decimal
 *        point: 160.0 with -2 -> "2", decpt 3); _ecvt_s with a buffer too small for ndigit. Guesses: ndigit 0 succeeds (r==0);
 *        negative ndigit for _ecvt_s is either rejected (EINVAL) or accepted (0) and never overruns; too-small buffer
 *        returns ERANGE or EINVAL with buf[0]==0.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

struct tb { char b[24]; char canary[8]; };
static void arm(struct tb *t) { int i; for (i = 0; i < 24; i++) t->b[i] = 'Q'; for (i = 0; i < 8; i++) t->canary[i] = (char)0x5a; }
static int canary_ok(const struct tb *t) { int i; for (i = 0; i < 8; i++) if (t->canary[i] != (char)0x5a) return 0; return 1; }

TC_CASE(ecvt_ndigit_zero)
{
    struct tb t; volatile double v = 1.5; int d = 99, s = 99; errno_t r; arm(&t);
    r = _ecvt_s(t.b, 20, v, 0, &d, &s);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(s, 0); CHECK(canary_ok(&t));
}
TC_CASE(ecvt_ndigit_negative)
{
    struct tb t; volatile double v = 1.5; int d = 99, s = 99; errno_t r; arm(&t);
    r = _ecvt_s(t.b, 20, v, -1, &d, &s);
    CHECK(r == 0 || r == EINVAL); CHECK(canary_ok(&t));
}
TC_CASE(fcvt_ndigit_negative)
{
    struct tb t; volatile double v = 160.0; int d = 99, s = 99; errno_t r; arm(&t);
    r = _fcvt_s(t.b, 20, v, -2, &d, &s);
    CHECK_EQ_I(r, 0); CHECK_STR(t.b, "2"); CHECK_EQ_I(d, 3); CHECK_EQ_I(s, 0); CHECK(canary_ok(&t));
}
TC_CASE(ecvt_buffer_small)
{
    struct tb t; volatile double v = 1.0; int d = 0, s = 0; errno_t r; arm(&t);
    r = _ecvt_s(t.b, 3, v, 5, &d, &s);
    CHECK(r == ERANGE || r == EINVAL); CHECK_EQ_I(t.b[0], 0); CHECK(canary_ok(&t));
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(ecvt_ndigit_zero); TC_RUN(ecvt_ndigit_negative); TC_RUN(fcvt_ndigit_negative); TC_RUN(ecvt_buffer_small);
    return tc_end();
}
