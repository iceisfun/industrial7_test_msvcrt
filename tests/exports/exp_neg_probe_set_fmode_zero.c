/* TC
 * id:    exp_neg_probe_set_fmode_zero
 * area:  exports
 * funcs: 
 * procs: _set_fmode _get_fmode
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _set_fmode: mode must be _O_TEXT or _O_BINARY
 * note:  PROBE: _set_fmode(0) is rejected (EINVAL + invalid parameter handler) and leaves _fmode unchanged. 0 is not _O_TEXT (0x4000) nor _O_BINARY (0x8000), but is the initial value of _fmode, so the real behaviour is not certain.
 */
#include "tc.h"
TC_DECL_PROC(int, _get_fmode, (int *));
TC_DECL_PROC(int, _set_fmode, (int));
TC_CASE(zero_mode_einval)
{
    int before = -1, after = -1;
    if (!tc_p__get_fmode || !tc_p__set_fmode) TC_SKIP("missing export");
    CHECK_EQ_I(tc_p__get_fmode(&before), 0);
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__set_fmode(0), 22);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(tc_p__get_fmode(&after), 0);
    CHECK_EQ_I(after, before);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_get_fmode); TC_LOAD_PROC(_set_fmode);
    if (!tc_iph_install()) return 2;
    TC_RUN(zero_mode_einval);
    return tc_end();
}
