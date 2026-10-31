/* TC
 * id:    loc_neg_wctomb_s_args
 * area:  locale
 * funcs: wctomb_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wctomb_s (error condition table)
 * note:  PROBE: (1) wctomb_s(&n, NULL, 0, L'A') is the state query: returns 0, *pRetValue==0, no invalid-parameter call.
 *        (2) NULL buffer with sizeInBytes>0 is EINVAL + one invalid-parameter call (MS docs error table, believed).
 *        (3) non-NULL buffer with sizeInBytes==0 fails (nonzero return, exact errno not asserted).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static char * volatile v_np = NULL;
static volatile size_t v_sz = 4;
static volatile size_t v_zero = 0;

TC_CASE(null_buffer_state_query)
{
    int r, n = 77, c0 = tc_iph_count;
    r = wctomb_s(&n, v_np, v_zero, L'A');
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(n, 0);
    CHECK_EQ_I(tc_iph_count, c0);
}
TC_CASE(null_buffer_positive_size_einval)
{
    int r, n = 77, c0 = tc_iph_count;
    r = wctomb_s(&n, v_np, v_sz, L'A');
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, c0 + 1);
}
TC_CASE(zero_size_nonnull_fails)
{
    char b[4] = { 'x', 'x', 'x', 'x' }; int n = 77, r;
    r = wctomb_s(&n, b, v_zero, L'A');
    CHECK(r != 0);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_buffer_state_query); TC_RUN(null_buffer_positive_size_einval); TC_RUN(zero_size_nonnull_fails); return tc_end(); }
