/* TC
 * id:    pff_neg_precision_overflow_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: snprintf sprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf width/precision specification (no stated limit)
 * note:  PROBES: a precision or width literal in the format string that does not fit in an int (2147483648, 99999999999)
 *        makes the format invalid: result -1, nothing written past the buffer. The invalid-parameter handler is NOT called (observed).
 *        sprintf_s additionally leaves buf[0]==0. errno value is not asserted. Canary bytes after the buffer must stay intact in every case.
 *        A (test bug): on Win10 22H2 19045 these return -1 but never call the invalid-parameter handler (tc_iph_count stays 0), even for sprintf_s.
 */
#include "tc.h"
#include <stdio.h>

struct tb { char b[32]; char canary[16]; };
static void arm(struct tb *t) { int i; for (i = 0; i < 32; i++) t->b[i] = 'Q'; for (i = 0; i < 16; i++) t->canary[i] = (char)0x5a; }
static int canary_ok(const struct tb *t) { int i; for (i = 0; i < 16; i++) if (t->canary[i] != (char)0x5a) return 0; return 1; }

TC_CASE(snprintf_prec_2p31)
{
    struct tb t; const char * volatile f = "%.2147483648f"; int r; arm(&t);
    tc_iph_count = 0;
    r = snprintf(t.b, 32, f, 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK(canary_ok(&t));
}
TC_CASE(sprintf_s_prec_huge)
{
    struct tb t; const char * volatile f = "%.99999999999f"; int r; arm(&t);
    tc_iph_count = 0;
    r = sprintf_s(t.b, 32, f, 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(t.b[0], 0); CHECK_EQ_I(tc_iph_count, 0); CHECK(canary_ok(&t));
}
TC_CASE(snprintf_width_2p31)
{
    struct tb t; const char * volatile f = "%2147483648f"; int r; arm(&t);
    tc_iph_count = 0;
    r = snprintf(t.b, 32, f, 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK(canary_ok(&t));
}
TC_CASE(snprintf_exp_prec_overflow)
{
    struct tb t; const char * volatile f = "%.4294967296e"; int r; arm(&t);
    tc_iph_count = 0;
    r = snprintf(t.b, 32, f, 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK(canary_ok(&t));
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(snprintf_prec_2p31); TC_RUN(sprintf_s_prec_huge); TC_RUN(snprintf_width_2p31); TC_RUN(snprintf_exp_prec_overflow);
    return tc_end();
}
