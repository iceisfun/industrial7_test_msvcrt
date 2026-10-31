/* TC
 * id:    stl_abs_basic
 * area:  stdlib
 * funcs: abs labs
 * procs: _abs64
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.6.1; MS docs abs, labs, llabs, _abs64
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

#undef _abs64
TC_DECL_PROC(__int64, _abs64, (__int64));
#define _abs64 tc_p__abs64

static volatile int vi; static volatile long vl; static volatile __int64 v64;

TC_CASE(pos)  { vi = 5; vl = 5; v64 = 5; CHECK_EQ_I(abs(vi), 5); CHECK_EQ_I(labs(vl), 5); CHECK_EQ_I(_abs64(v64), 5); }
TC_CASE(neg)  { vi = -5; vl = -5; v64 = -5; CHECK_EQ_I(abs(vi), 5); CHECK_EQ_I(labs(vl), 5); CHECK_EQ_I(_abs64(v64), 5); }
TC_CASE(zero) { vi = 0; vl = 0; v64 = 0; CHECK_EQ_I(abs(vi), 0); CHECK_EQ_I(labs(vl), 0); CHECK_EQ_I(_abs64(v64), 0); }
TC_CASE(labs_neg) { vl = -2147483647L; CHECK_EQ_I(labs(vl), 2147483647L); vl = -123456L; CHECK_EQ_I(labs(vl), 123456L); }
TC_CASE(abs64_neg_large) { v64 = -5000000000LL; CHECK_EQ_I(_abs64(v64), 5000000000LL); v64 = 5000000000LL; CHECK_EQ_I(_abs64(v64), 5000000000LL); }
TC_CASE(near_min) { vi = INT_MIN + 1; CHECK_EQ_I(abs(vi), INT_MAX); vl = LONG_MIN + 1; CHECK_EQ_I(labs(vl), LONG_MAX); v64 = -9223372036854775807LL; CHECK_EQ_I(_abs64(v64), 9223372036854775807LL); }
TC_CASE(widths)
{
    vi = -1; vl = -1; v64 = -1;
    CHECK_EQ_U(sizeof(abs(vi)), sizeof(int));
    CHECK_EQ_U(sizeof(labs(vl)), sizeof(long));
    CHECK_EQ_U(sizeof(_abs64(v64)), 8);
    v64 = -4294967297LL; CHECK_EQ_I(_abs64(v64), 4294967297LL); /* would be wrong if truncated to 32 bits */
}

int main(void) { tc_begin(); TC_LOAD_PROC(_abs64); if (!(tc_p__abs64)) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; } TC_RUN(pos); TC_RUN(neg); TC_RUN(zero); TC_RUN(labs_neg); TC_RUN(abs64_neg_large); TC_RUN(near_min); TC_RUN(widths); return tc_end(); }
