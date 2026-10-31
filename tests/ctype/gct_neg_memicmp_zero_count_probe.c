/* TC
 * id:    gct_neg_memicmp_zero_count_probe
 * area:  ctype
 * funcs: _memicmp
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _memicmp
 * note:  PROBE: _memicmp(NULL,NULL,0) is believed to return 0 without calling the invalid parameter handler
 *        (ucrt checks count==0 before validating pointers).
 */
#include "tc.h"
#include <string.h>

static const char * volatile nul = NULL;
TC_CASE(null_zero_count)
{
    int r; tc_iph_count = 0;
    r = _memicmp(nul, nul, 0);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(null_zero_count);
    return tc_end();
}
