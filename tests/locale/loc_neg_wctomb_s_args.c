/* TC
 * id:    loc_neg_wctomb_s_args
 * area:  locale
 * funcs: wctomb_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs wctomb_s (error condition table)
 * note:  Win10 triage (class B, probe -> observed): on ucrtbase 19045 (NULL,0) returns 0/*pRet=1, (NULL,4) returns 0/*pRet=0 with no handler call, and a
 *        non-NULL buffer with size 0 fails (ERANGE, handler called). Observed on Windows 10 22H2 build 19045.
 *        Original PROBE: (1) wctomb_s(&n, NULL, 0, L'A') is the state query: returns 0, *pRetValue==0, no invalid-parameter call.
 *        (2) NULL buffer with sizeInBytes>0 is EINVAL + one invalid-parameter call (MS docs error table, believed).
 *        (3) non-NULL buffer with sizeInBytes==0 fails (nonzero return, exact errno not asserted).
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 *        [class B] deviates from the runtime-constraint rules of C11 Annex K (K.3.6.x / K.3.9.3.x) that the MS docs mirror; expectation = Windows behavior.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static char * volatile v_np = NULL;
static volatile size_t v_sz = 4;
static volatile size_t v_zero = 0;

/* Win10 22H2 ucrtbase (observed): the "state query" is (NULL buffer, sizeInBytes > 0): returns 0 with *pRetValue == 0, no handler call.
 * (NULL buffer, size 0) returns 0 with *pRetValue == 1; neither is an error. Both deviate from the MS docs error table (EINVAL for NULL buffer + size > 0). */
TC_CASE(null_buffer_zero_size)
{
    int r, n = 77, c0 = tc_iph_count;
    r = wctomb_s(&n, v_np, v_zero, L'A');
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(n, 1);
    CHECK_EQ_I(tc_iph_count, c0);
}
TC_CASE(null_buffer_positive_size_state_query)
{
    int r, n = 77, c0 = tc_iph_count;
    r = wctomb_s(&n, v_np, v_sz, L'A');
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(n, 0);
    CHECK_EQ_I(tc_iph_count, c0);
}
TC_CASE(zero_size_nonnull_fails)
{
    char b[4] = { 'x', 'x', 'x', 'x' }; int n = 77, r;
    r = wctomb_s(&n, b, v_zero, L'A');
    CHECK(r != 0);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_buffer_zero_size); TC_RUN(null_buffer_positive_size_state_query); TC_RUN(zero_size_nonnull_fails); return tc_end(); }
