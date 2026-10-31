/* TC
 * id:    stl_llabs_basic
 * area:  stdlib
 * funcs: 
 * procs: llabs
 * crt:   >=120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.6.1; MS docs abs, labs, llabs
 * note:  llabs is exported from msvcr120 on; msvcr100 and older lack it (see stl_llabs_lldiv_exported_probe)
 */
#include "tc.h"
#include <stdlib.h>

#undef llabs
TC_DECL_PROC(long long, llabs, (long long));
#define llabs tc_p_llabs

static volatile long long v;

TC_CASE(pos)  { v = 7; CHECK_EQ_I(llabs(v), 7); }
TC_CASE(neg)  { v = -7; CHECK_EQ_I(llabs(v), 7); }
TC_CASE(zero) { v = 0; CHECK_EQ_I(llabs(v), 0); }
TC_CASE(large_neg) { v = -(1LL << 62); CHECK_EQ_I(llabs(v), 1LL << 62); v = -4294967297LL; CHECK_EQ_I(llabs(v), 4294967297LL); }

int main(void) { tc_begin(); TC_LOAD_PROC(llabs); if (!(tc_p_llabs)) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; } TC_RUN(pos); TC_RUN(neg); TC_RUN(zero); TC_RUN(large_neg); return tc_end(); }
