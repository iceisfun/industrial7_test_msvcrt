/* TC
 * id:    stl_neg_abs_min_no_errno_no_iph
 * area:  stdlib
 * kind:  negative
 * funcs: abs labs
 * procs: llabs _abs64
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs abs, labs, llabs, _abs64: no error return; result for the most negative value is not representable
 * note:  Only asserts that the most-negative input raises no invalid-parameter call and leaves errno alone; the returned value is
 *        computed but not asserted. llabs is resolved dynamically (not in every import library; skipped when not exported).
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

TC_DECL_PROC(long long, llabs, (long long));
TC_DECL_PROC(__int64, _abs64, (__int64));
static volatile int v_int = INT_MIN;
static volatile long v_long = LONG_MIN;
static volatile __int64 v_i64 = (-0x7fffffffffffffffLL - 1);

TC_CASE(abs_int_min)  { volatile int r; tc_iph_count = 0; errno = 0; r = abs(v_int); (void)r; CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(labs_long_min){ volatile long r; tc_iph_count = 0; errno = 0; r = labs(v_long); (void)r; CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(abs64_min)    { volatile __int64 r; if (!tc_p__abs64) TC_SKIP("_abs64 not exported"); tc_iph_count = 0; errno = 0; r = tc_p__abs64(v_i64); (void)r; CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(llabs_min)
{
    volatile long long r;
    if (!tc_p_llabs) TC_SKIP("llabs not exported");
    tc_iph_count = 0; errno = 0; r = tc_p_llabs(v_i64); (void)r;
    CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(control_min_plus_one)
{
    volatile int a = INT_MIN + 1; volatile long b = LONG_MIN + 1; volatile __int64 c = v_i64 + 1;
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(abs(a), INT_MAX); CHECK_EQ_I(labs(b), LONG_MAX); if (tc_p__abs64) CHECK_EQ_I(tc_p__abs64(c), 0x7fffffffffffffffLL);
    CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(llabs); TC_LOAD_PROC(_abs64); tc_iph_install();
    TC_RUN(abs_int_min); TC_RUN(labs_long_min); TC_RUN(abs64_min); TC_RUN(llabs_min); TC_RUN(control_min_plus_one);
    return tc_end();
}
