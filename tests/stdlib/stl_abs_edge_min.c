/* TC
 * id:    stl_abs_edge_min
 * area:  stdlib
 * funcs: abs labs
 * procs: _abs64
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.22.6.1 (UB for most-negative); MS docs abs
 * note:  PROBE: abs(INT_MIN) etc. are undefined in C; MS docs only say the result is undefined/not representable. Probing that real CRTs return the input unchanged (two's complement wrap). Documented near-min values live in stl_abs_basic.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

#undef _abs64
TC_DECL_PROC(__int64, _abs64, (__int64));
#define _abs64 tc_p__abs64

static volatile int vi; static volatile long vl; static volatile __int64 v64;

TC_CASE(abs_int_min)  { vi = INT_MIN; CHECK_EQ_I(abs(vi), INT_MIN); }
TC_CASE(labs_long_min) { vl = LONG_MIN; CHECK_EQ_I(labs(vl), LONG_MIN); }
TC_CASE(abs64_min) { v64 = (-9223372036854775807LL - 1); CHECK_EQ_I(_abs64(v64), (-9223372036854775807LL - 1)); }

int main(void) { tc_begin(); TC_LOAD_PROC(_abs64); if (!(tc_p__abs64)) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; } TC_RUN(abs_int_min); TC_RUN(labs_long_min); TC_RUN(abs64_min); return tc_end(); }
