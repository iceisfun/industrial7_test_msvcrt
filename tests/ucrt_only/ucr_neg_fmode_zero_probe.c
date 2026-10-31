/* TC
 * id:    ucr_neg_fmode_zero_probe
 * area:  ucrt_only
 * funcs: _set_fmode _get_fmode
 * crt:   win10
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _set_fmode (valid modes are _O_TEXT, _O_BINARY, and the Unicode modes)
 * note:  PROBE: _set_fmode(0) is rejected (EINVAL, handler called once) because 0 is none of the valid mode flags,
 *        and the previous mode is unchanged.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
static volatile int vmode;
TC_CASE(zero)
{
    int r, before = -2, after = -3;
    CHECK_EQ_I(_set_fmode(_O_TEXT), 0);
    CHECK_EQ_I(_get_fmode(&before), 0);
    vmode = 0; errno = 0; tc_iph_count = 0; r = _set_fmode(vmode);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_get_fmode(&after), 0); CHECK_EQ_I(after, before);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(zero);
    return tc_end();
}
