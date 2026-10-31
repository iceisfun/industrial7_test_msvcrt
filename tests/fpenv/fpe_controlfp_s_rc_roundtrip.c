/* TC
 * id:    fpe_controlfp_s_rc_roundtrip
 * area:  fpenv
 * funcs: _controlfp
 * procs: _controlfp_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _controlfp_s
 * note:  _controlfp_s is resolved dynamically because old mingw import libs lack it.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>
TC_DECL_PROC(int, _controlfp_s, (unsigned int *, unsigned int, unsigned int));

TC_CASE(four_modes)
{
    static const unsigned int modes[4] = { _RC_NEAR, _RC_DOWN, _RC_UP, _RC_CHOP };
    unsigned int orig = 0, cur = 0xdeadbeef; int i;
    CHECK_EQ_I(tc_p__controlfp_s(&orig, 0, 0), 0);
    for (i = 0; i < 4; i++) {
        cur = 0xdeadbeef;
        CHECK_EQ_I(tc_p__controlfp_s(&cur, modes[i], _MCW_RC), 0);
        CHECK_EQ_U(cur & _MCW_RC, modes[i]);
        CHECK_EQ_U(_controlfp(0, 0) & _MCW_RC, modes[i]);
    }
    tc_p__controlfp_s(&cur, orig, _MCW_RC);
}
TC_CASE(restore)
{
    unsigned int orig = 0, cur = 0;
    tc_p__controlfp_s(&orig, 0, 0);
    tc_p__controlfp_s(&cur, _RC_CHOP, _MCW_RC);
    CHECK_EQ_I(tc_p__controlfp_s(&cur, orig, _MCW_RC), 0);
    CHECK_EQ_U(cur & _MCW_RC, orig & _MCW_RC);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_controlfp_s); if (!tc_p__controlfp_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; } TC_RUN(four_modes); TC_RUN(restore); return tc_end(); }
