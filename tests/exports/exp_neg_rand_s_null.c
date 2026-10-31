/* TC
 * id:    exp_neg_rand_s_null
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: rand_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs rand_s: NULL randomValue -> invalid parameter handler, returns EINVAL
 * note:  
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(errno_t, rand_s, (unsigned int *));

TC_CASE(null_returns_einval) { CHECK_EQ_I(tc_p_rand_s(NULL), EINVAL); }
TC_CASE(iph_called) { tc_iph_count = 0; tc_p_rand_s(NULL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(valid_ptr_returns_0) { unsigned int v = 0; CHECK_EQ_I(tc_p_rand_s(&v), 0); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(rand_s); tc_iph_install();
    if (!tc_p_rand_s) { tc_write("TC FAIL " TC_TEST_ID "::exported\n"); return 1; }
    TC_RUN(null_returns_einval); TC_RUN(iph_called); TC_RUN(valid_ptr_returns_0);
    return tc_end();
}
