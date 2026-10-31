/* TC
 * id:    scn_neg_snscanf_count_zero_probe
 * area:  scanf
 * kind:  negative
 * funcs: _snscanf
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snscanf (parameter validation: input NULL -> invalid parameter handler, errno EINVAL)
 * note:  PROBE: _snscanf(NULL, 0, "%d", &x). Probed: result is EOF, x untouched (case result), and whether the
 *        invalid-parameter handler runs exactly once (case iph; likely, since the NULL-input validation precedes
 *        the count test). errno is not asserted.
 */
#include "tc.h"
#include <stdio.h>

static volatile size_t cnt;
static const char *volatile nullsrc;

TC_CASE(result)
{
    int x = 5, r; tc_iph_install(); nullsrc = NULL; cnt = 0;
    r = _snscanf(nullsrc, cnt, "%d", &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(x, 5);
}
TC_CASE(iph)
{
    int x = 5; tc_iph_install(); nullsrc = NULL; cnt = 0; tc_iph_count = 0;
    (void)_snscanf(nullsrc, cnt, "%d", &x);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(result); TC_RUN(iph); return tc_end(); }
