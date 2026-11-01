/* TC
 * id:    pfm_2_neg_conv_percent_trailing_snprintf_probe
 * area:  stdio_printf
 * funcs: sprintf _snprintf
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs printf format specification (incomplete specification -> invalid parameter)
 * note:  OBSERVED: sprintf("abc%") and _snprintf(b, n, "x%") on ucrtbase return 3 / 1 (text kept), errno 0, no invalid-parameter call (no format validation in the non-_s variants).
 *        Observed on Win10 22H2 19045 ucrtbase: the non-_s printf family does not validate format strings (no handler call, no errno); the bad conversion char is copied through.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static char g_b[32];
TC_CASE(sprintf_abc) { char *b = g_b; int r; const char * volatile f = "abc%"; tc_iph_count = 0; errno = 0; r = sprintf(b, f);
    CHECK_EQ_I(r, 3); CHECK_STR(g_b, "abc"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(snprintf_x)  { char *b = g_b; int r; const char * volatile f = "x%"; tc_iph_count = 0; errno = 0; r = _snprintf(b, sizeof b, f);
    CHECK_EQ_I(r, 1); CHECK_STR(g_b, "x"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_abc); TC_RUN(snprintf_x);
    return tc_end();
}
