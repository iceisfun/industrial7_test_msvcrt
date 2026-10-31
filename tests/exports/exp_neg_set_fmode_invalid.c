/* TC
 * id:    exp_neg_set_fmode_invalid
 * area:  exports
 * funcs: 
 * procs: _set_fmode _get_fmode _errno
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_fmode/_get_fmode: mode must be _O_TEXT or _O_BINARY, else EINVAL + invalid parameter handler; _get_fmode(NULL) likewise
 * note:  _set_fmode(0) is deliberately NOT asserted here (0 is the initial _fmode in UCRT; see exp_neg_probe_set_fmode_zero).
 */
#include "tc.h"


TC_DECL_PROC(int, _get_fmode, (int *));
TC_DECL_PROC(int, _set_fmode, (int));
TC_DECL_PROC(int *, _errno, (void));

TC_CASE(bad_mode_einval)
{
    int before = -1, after = -1, r;
    if (!tc_p__get_fmode || !tc_p__set_fmode) TC_SKIP("missing export");
    CHECK_EQ_I(tc_p__get_fmode(&before), 0);
    tc_iph_count = 0;
    if (tc_p__errno) *tc_p__errno() = 0;
    r = tc_p__set_fmode(0x1234);
    CHECK_EQ_I(r, 22);
    CHECK_EQ_I(tc_iph_count, 1);
    if (tc_p__errno) CHECK_EQ_I(*tc_p__errno(), 22);
    CHECK_EQ_I(tc_p__get_fmode(&after), 0);
    CHECK_EQ_I(after, before);   /* unchanged */
}
TC_CASE(get_null_einval)
{
    if (!tc_p__get_fmode) TC_SKIP("missing export");
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__get_fmode(NULL), 22);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_get_fmode); TC_LOAD_PROC(_set_fmode); TC_LOAD_PROC(_errno);
    if (!tc_iph_install()) return 2;
    TC_RUN(bad_mode_einval); TC_RUN(get_null_einval);
    return tc_end();
}
