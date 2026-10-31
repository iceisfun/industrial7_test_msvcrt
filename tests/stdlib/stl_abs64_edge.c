/* TC
 * id:    stl_abs64_edge
 * area:  stdlib
 * funcs:
 * procs: _abs64
 * crt:   >=140
 * tier:  p1
 * conf:  documented
 * ref:   MS docs abs, _abs64
 * note:  _abs64 is inline in mingw headers, so it is resolved from ucrtbase at run time.
 */
#include "tc.h"

TC_DECL_PROC(__int64, _abs64, (__int64));

TC_CASE(values)
{
    if (!tc_p__abs64) TC_SKIP("_abs64 not exported");
    CHECK_EQ_I(tc_p__abs64(-9223372036854775807LL), 9223372036854775807LL);
    CHECK_EQ_I(tc_p__abs64(9223372036854775807LL), 9223372036854775807LL);
    CHECK_EQ_I(tc_p__abs64(0), 0);
    CHECK_EQ_I(tc_p__abs64(-5000000000LL), 5000000000LL);
}
TC_CASE(exported) { CHECK(tc_p__abs64 != NULL); }
int main(void) { tc_begin(); TC_LOAD_PROC(_abs64); TC_RUN(exported); TC_RUN(values); return tc_end(); }
