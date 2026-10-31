/* TC
 * id:    dat_neg_fmode_set_invalid
 * area:  data_exports
 * funcs: _set_fmode _get_fmode
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_fmode: returns EINVAL if mode is not a supported translation mode and invokes the invalid parameter handler
 */
#include "tc.h"
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

static int cur(void) { int m = -99; _get_fmode(&m); return m; }
static int before;
static void bad(int mode, int expect_iph_total_after)
{
    int r;
    errno = 0;
    r = _set_fmode(mode);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, expect_iph_total_after);
}
TC_CASE(mode_0x1234_einval)    { bad(0x1234, 1); }
TC_CASE(mode_negative_einval)  { bad(-1, 2); }
TC_CASE(mode_O_APPEND_einval)  { bad(_O_APPEND, 3); }
TC_CASE(iph_called_each)       { CHECK_EQ_I(tc_iph_count, 3); }
TC_CASE(fmode_unchanged_after_failure) { CHECK_EQ_I(cur(), before); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL dat_neg_fmode_set_invalid::__setup no iph\n"); return 1; }
    before = cur();
    TC_RUN(mode_0x1234_einval); TC_RUN(mode_negative_einval); TC_RUN(mode_O_APPEND_einval);
    TC_RUN(iph_called_each); TC_RUN(fmode_unchanged_after_failure);
    return tc_end();
}
