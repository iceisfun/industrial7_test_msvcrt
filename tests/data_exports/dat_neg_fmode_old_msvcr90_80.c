/* TC
 * id:    dat_neg_fmode_old_msvcr90_80
 * area:  data_exports
 * funcs: 
 * procs: _get_fmode _set_fmode
 * crt:   msvcr90 msvcr80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_fmode/_get_fmode (VS2005+): EINVAL on bad parameter
 */
#include "tc.h"
#include <errno.h>
TC_DECL_PROC(int, _get_fmode, (int *));
TC_DECL_PROC(int, _set_fmode, (int));

static int cur(void) { int m = -99; tc_p__get_fmode(&m); return m; }
TC_CASE(get_null)
{ int c, r; if (!tc_p__get_fmode) TC_SKIP("no proc"); c = tc_iph_count; r = tc_p__get_fmode((int *)(volatile void *)0);
  CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, c + 1); }
TC_CASE(set_bad)
{ int b, c, r; if (!tc_p__set_fmode || !tc_p__get_fmode) TC_SKIP("no proc"); b = cur(); c = tc_iph_count;
  r = tc_p__set_fmode(0x1234); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(cur(), b);
  r = tc_p__set_fmode(-1); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(cur(), b); CHECK_EQ_I(tc_iph_count, c + 2); }

int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(_get_fmode); TC_LOAD_PROC(_set_fmode);
    TC_RUN(get_null); TC_RUN(set_bad);
    return tc_end();
}
