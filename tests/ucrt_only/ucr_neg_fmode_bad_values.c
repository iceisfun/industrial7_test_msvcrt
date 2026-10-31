/* TC
 * id:    ucr_neg_fmode_bad_values
 * area:  ucrt_only
 * funcs: _set_fmode _get_fmode
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_fmode / _get_fmode: invalid mode or NULL pointer -> invalid parameter handler, EINVAL
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static volatile int vmode;
static int * volatile vnull;
TC_CASE(bad_mode)
{
    int r, m = -1, m0 = -2;
    CHECK_EQ_I(_get_fmode(&m0), 0);
    vmode = 0x1234; errno = 0; tc_iph_count = 0; r = _set_fmode(vmode);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    vmode = -1; errno = 0; tc_iph_count = 0; r = _set_fmode(vmode);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_get_fmode(&m), 0); CHECK_EQ_I(m, m0);  /* failed set leaves the mode unchanged */
}
TC_CASE(get_null) { int r; errno = 0; tc_iph_count = 0; r = _get_fmode(vnull); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(bad_mode); TC_RUN(get_null);
    return tc_end();
}
