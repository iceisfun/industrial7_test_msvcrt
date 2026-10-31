/* TC
 * id:    gct_neg_toascii_negative_and_large_probe
 * area:  ctype
 * funcs: 
 * procs: __toascii toascii
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs toascii (no domain restriction is documented)
 * note:  PROBE: toascii(c) is a plain mask c & 0x7F even for negative / huge ints; no invalid-parameter handler call and errno untouched.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
TC_DECL_PROC(int, __toascii, (int));
TC_DECL_PROC(int, toascii, (int));
static volatile int v;

TC_CASE(negative_one) { int e; tc_iph_install(); tc_iph_count = 0; errno = 0; v = -1; CHECK_EQ_I(tc_p___toascii(v), 0x7F); e = errno; CHECK_EQ_I(e, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(extremes)
{
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    v = 0x7FFFFFFF; CHECK_EQ_I(tc_p___toascii(v), 0x7F);
    v = INT_MIN; CHECK_EQ_I(tc_p___toascii(v), 0);
    v = -128; CHECK_EQ_I(tc_p___toascii(v), 0);
    CHECK_EQ_I(tc_iph_count, 0);
    { int e = errno; CHECK_EQ_I(e, 0); }
}
TC_CASE(undecorated)
{
    if (!tc_p_toascii) TC_SKIP("undecorated alias not exported");
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    v = -1; CHECK_EQ_I(tc_p_toascii(v), 0x7F);
    v = INT_MIN; CHECK_EQ_I(tc_p_toascii(v), 0);
    CHECK_EQ_I(tc_iph_count, 0);
    { int e = errno; CHECK_EQ_I(e, 0); }
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(__toascii); TC_LOAD_PROC(toascii); if (!tc_p___toascii) { tc_write("TC FAIL " TC_TEST_ID "::loaded __toascii missing\n"); return 1; }
    TC_RUN(negative_one); TC_RUN(extremes); TC_RUN(undecorated);
    return tc_end();
}
