/* TC
 * id:    scn_neg_sscanf_bad_conversion_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.21.6.2p13 (invalid conversion specification: behaviour undefined); MS docs "Parameter Validation" (format errors invoke the handler)
 * note:  PROBES an unknown conversion specifier ('%y'), a lone '%' at end of format and a truncated '%5' spec.
 *        Asserted (guess): nothing is stored, return is not positive, the invalid-parameter handler runs exactly once, errno == EINVAL.
 *        The "safe" cases (no store, no crash, ret <= 0) are separate from the iph/errno guesses.
 *        CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31): unknown/truncated conversion specifiers are a plain matching failure: return 0, handler NOT called, errno untouched (0). Stable (2 runs).
 *        Observed on Win10 22H2 ucrtbase only; the same expectation on the other CRTs (>=80) is [unverified] and must be confirmed when those CRTs are run in the VM.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile in;
static const char *volatile fmt;

static int run(const char *i, const char *f, int *x)
{
    int r; tc_iph_count = 0; errno = 0; r = sscanf(i, f, x); return r;
}

TC_CASE(unknown_y_safe)   { int x = 77, r; r = run("5", "%y", &x); CHECK(r <= 0); CHECK_EQ_I(x, 77); }
TC_CASE(percent_end_safe) { int x = 77, r; r = run("5", "%", &x); CHECK(r <= 0); CHECK_EQ_I(x, 77); }
TC_CASE(truncated_5_safe) { int x = 77, r; r = run("5", "%5", &x); CHECK(r <= 0); CHECK_EQ_I(x, 77); }
TC_CASE(unknown_y_iph)    { int x = 77; run("5", "%y", &x); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(percent_end_iph)  { int x = 77; run("5", "%", &x); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(truncated_5_iph)  { int x = 77; run("5", "%5", &x); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP " TC_TEST_ID " no handler API\n"); return tc_end(); }
    TC_RUN(unknown_y_safe); TC_RUN(percent_end_safe); TC_RUN(truncated_5_safe);
    TC_RUN(unknown_y_iph); TC_RUN(percent_end_iph); TC_RUN(truncated_5_iph);
    return tc_end();
}
