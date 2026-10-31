/* TC
 * id:    fpe_neg_control87_pc_ignored_x64
 * area:  fpenv
 * funcs: _control87
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _control87: _MCW_PC is not supported on x64 platforms
 * note:  Arithmetic-only assertions, as for _controlfp.
 */
#include "tc.h"
#include <float.h>

static volatile double v_one = 1.0, v_three = 3.0, v_tiny = 1e-10;
static double third(void) { return v_one / v_three; }
static double onep(void)  { return v_one + v_tiny; }

TC_CASE(pc24)
{
    unsigned orig; double a0, b0;
    if (!TC_X64) TC_SKIP("x64 only");
    orig = _control87(0, 0);
    a0 = third(); b0 = onep();
    _control87(_PC_24, _MCW_PC);
    CHECK_BITS_D(third(), a0);
    CHECK_BITS_D(onep(), b0);
    CHECK(onep() != 1.0);
    _control87(orig, _MCW_EM | _MCW_RC);
}
TC_CASE(pc53)
{
    unsigned orig;
    if (!TC_X64) TC_SKIP("x64 only");
    orig = _control87(0, 0);
    _control87(_PC_53, _MCW_PC);
    CHECK_BITS_D(third(), 0.3333333333333333);
    CHECK(onep() != 1.0);
    _control87(orig, _MCW_EM | _MCW_RC);
}
int main(void) { tc_begin(); TC_RUN(pc24); TC_RUN(pc53); return tc_end(); }
