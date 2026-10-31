/* TC
 * id:    fpe_neg_controlfp_ic_ignored_x64
 * area:  fpenv
 * funcs: _controlfp
 * procs: 
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _controlfp: _MCW_IC unsupported on x64
 * note:  Probe: IC field readback after _controlfp(_IC_PROJECTIVE,_MCW_IC) is unknown on x64, so only 'unchanged vs before' and unaffected arithmetic are asserted.
 */
#include "tc.h"
#include <float.h>

static volatile double v_one = 1.0, v_three = 3.0, v_tiny = 1e-10;
static double third(void) { return v_one / v_three; }
static double onep(void)  { return v_one + v_tiny; }

TC_CASE(ic_readback)
{
    unsigned orig, before, after;
    if (!TC_X64) TC_SKIP("x64 only");
    orig = _controlfp(0, 0);
    before = orig & _MCW_IC;
    after = _controlfp(_IC_PROJECTIVE, _MCW_IC) & _MCW_IC;
    CHECK_EQ_U(after, before);
    _controlfp(orig, _MCW_EM | _MCW_RC);
}
TC_CASE(arith_unaffected)
{
    unsigned orig;
    if (!TC_X64) TC_SKIP("x64 only");
    orig = _controlfp(0, 0);
    _controlfp(_IC_PROJECTIVE, _MCW_IC);
    CHECK_BITS_D(third(), 0.3333333333333333);
    CHECK(onep() != 1.0);
    _controlfp(orig, _MCW_EM | _MCW_RC);
}
int main(void) { tc_begin(); TC_RUN(ic_readback); TC_RUN(arith_unaffected); return tc_end(); }
