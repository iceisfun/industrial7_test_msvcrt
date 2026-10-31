/* TC
 * id:    pff_neg_gcvt_s_digits_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _gcvt_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _gcvt_s (EINVAL for NULL buffer / size 0; buffer-too-small error)
 * note:  PROBES the digits argument edge cases of _gcvt_s, which the docs do not spell out: digits < 0, digits == 0,
 *        digits >= buffer size. Guesses: digits=-1 -> EINVAL with IPH 1 and buf[0]==0; digits >= size (10,10 and 5,5)
 *        -> EINVAL or ERANGE with buf[0]==0 and IPH 1; digits==0 -> not a crash, r is 0 or EINVAL. Canary after the
 *        buffer must be intact in all cases.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

struct tb { char b[16]; char canary[8]; };
static void arm(struct tb *t) { int i; for (i = 0; i < 16; i++) t->b[i] = 'Q'; for (i = 0; i < 8; i++) t->canary[i] = (char)0x5a; }
static int canary_ok(const struct tb *t) { int i; for (i = 0; i < 8; i++) if (t->canary[i] != (char)0x5a) return 0; return 1; }

TC_CASE(digits_negative)
{
    struct tb t; volatile double v = 1.5; errno_t r; arm(&t); tc_iph_count = 0;
    r = _gcvt_s(t.b, 10, v, -1);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(t.b[0], 0); CHECK(canary_ok(&t));
}
TC_CASE(digits_zero)
{
    struct tb t; volatile double v = 1.5; errno_t r; arm(&t); tc_iph_count = 0;
    r = _gcvt_s(t.b, 10, v, 0);
    CHECK(r == 0 || r == EINVAL); CHECK(canary_ok(&t));
}
TC_CASE(digits_equal_size_10)
{
    struct tb t; volatile double v = 1.5; errno_t r; arm(&t); tc_iph_count = 0;
    r = _gcvt_s(t.b, 10, v, 10);
    CHECK(r == EINVAL || r == ERANGE); CHECK_EQ_I(t.b[0], 0); CHECK_EQ_I(tc_iph_count, 1); CHECK(canary_ok(&t));
}
TC_CASE(digits_equal_size_5)
{
    struct tb t; volatile double v = 123456.0; errno_t r; arm(&t); tc_iph_count = 0;
    r = _gcvt_s(t.b, 5, v, 5);
    CHECK(r == EINVAL || r == ERANGE); CHECK_EQ_I(t.b[0], 0); CHECK_EQ_I(tc_iph_count, 1); CHECK(canary_ok(&t));
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(digits_negative); TC_RUN(digits_zero); TC_RUN(digits_equal_size_10); TC_RUN(digits_equal_size_5);
    return tc_end();
}
