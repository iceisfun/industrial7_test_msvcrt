/* TC
 * id:    fpe_controlfp_s_em_roundtrip
 * area:  fpenv
 * funcs: _controlfp
 * procs: _controlfp_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _controlfp_s

 */
#include "tc.h"
#include <float.h>
#include <errno.h>
TC_DECL_PROC(int, _controlfp_s, (unsigned int *, unsigned int, unsigned int));

#define TC_EM5 0x1f /* the five IEEE exception mask bits; excludes x64-only _EM_DENORMAL */
static const unsigned int bits[5] = { _EM_INVALID, _EM_ZERODIVIDE, _EM_OVERFLOW, _EM_UNDERFLOW, _EM_INEXACT };

TC_CASE(unmask_remask_each)
{
    unsigned int orig = 0, cur = 0; int i;
    tc_p__controlfp_s(&orig, 0, 0);
    for (i = 0; i < 5; i++) {
        _clearfp();
        CHECK_EQ_I(tc_p__controlfp_s(&cur, TC_EM5, TC_EM5), 0);
        CHECK_EQ_I(tc_p__controlfp_s(&cur, 0, bits[i]), 0);
        CHECK_EQ_U(cur & bits[i], 0);
        CHECK_EQ_U(_controlfp(0, 0) & TC_EM5, TC_EM5 & ~bits[i]);
        CHECK_EQ_I(tc_p__controlfp_s(&cur, bits[i], bits[i]), 0);
        CHECK_EQ_U(cur & bits[i], bits[i]);
        CHECK_EQ_U(_controlfp(0, 0) & TC_EM5, TC_EM5);
    }
    tc_p__controlfp_s(&cur, orig, TC_EM5);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_controlfp_s); if (!tc_p__controlfp_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; } TC_RUN(unmask_remask_each); return tc_end(); }
