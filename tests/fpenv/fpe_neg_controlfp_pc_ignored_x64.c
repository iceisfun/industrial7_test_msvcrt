/* TC
 * id:    fpe_neg_controlfp_pc_ignored_x64
 * area:  fpenv
 * funcs: _controlfp
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _control87: _MCW_PC is not supported on x64 platforms
 * note:  Only arithmetic is asserted: SSE2 double math has no precision-control field, so requesting 24-bit precision cannot alter results. Control-word readback is deliberately not asserted.
 */
#include "tc.h"
#include <float.h>

static volatile double v_one = 1.0, v_three = 3.0, v_tiny = 1e-10;
static double third(void) { return v_one / v_three; }
static double onep(void)  { return v_one + v_tiny; }

TC_CASE(pc24_no_effect)
{
    unsigned orig; double a0, b0;
    if (!TC_X64) TC_SKIP("x64 only");
    orig = _controlfp(0, 0);
    a0 = third(); b0 = onep();
    _controlfp(_PC_24, _MCW_PC);
    CHECK_BITS_D(third(), a0);
    CHECK_BITS_D(onep(), b0);
    CHECK(onep() != 1.0);
    CHECK_BITS_D(third(), 0.3333333333333333);
    _controlfp(orig, _MCW_EM | _MCW_RC);
}
TC_CASE(restore)
{
    unsigned orig, r;
    if (!TC_X64) TC_SKIP("x64 only");
    orig = _controlfp(0, 0);
    _controlfp(_PC_24, _MCW_PC);
    _controlfp(orig, _MCW_EM | _MCW_RC);
    r = _controlfp(0, 0);
    CHECK_EQ_U(r & (_MCW_EM | _MCW_RC), orig & (_MCW_EM | _MCW_RC));
    CHECK_BITS_D(third(), 0.3333333333333333);
}
int main(void) { tc_begin(); TC_RUN(pc24_no_effect); TC_RUN(restore); return tc_end(); }
