/* TC
 * id:    fpe_fpreset_restores_default
 * area:  fpenv
 * funcs: _controlfp
 * procs: _fpreset
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _fpreset (reinitializes the floating-point package)
 * note:  PROBE: after _fpreset the rounding control is _RC_NEAR and all exceptions are masked again. The docs only say 'reinitializes'; both are the expected FNINIT/default state.
 */
#include "tc.h"
#include <float.h>

typedef void (__cdecl *fpreset_t)(void);
static fpreset_t p_fpreset;

TC_CASE(exported) { CHECK(p_fpreset != NULL); }
TC_CASE(rc_near_after_reset)
{
    unsigned orig = _controlfp(0, 0);
    if (!p_fpreset) TC_SKIP("no _fpreset");
    _controlfp(_RC_UP, _MCW_RC);
    CHECK_EQ_U(_controlfp(0, 0) & _MCW_RC, _RC_UP);
    p_fpreset();
    CHECK_EQ_U(_controlfp(0, 0) & _MCW_RC, _RC_NEAR);
    _controlfp(orig, _MCW_RC | _MCW_EM);
}
TC_CASE(em_masked_after_reset)
{
    unsigned orig = _controlfp(0, 0);
    if (!p_fpreset) TC_SKIP("no _fpreset");
    _controlfp(0, _EM_INEXACT);      /* unmask inexact only; no FP op in between */
    CHECK_EQ_U(_controlfp(0, 0) & _EM_INEXACT, 0);
    p_fpreset();
    CHECK_EQ_U(_controlfp(0, 0) & _MCW_EM & 0x1F, 0x1F);
    _controlfp(orig, _MCW_RC | _MCW_EM);
}
int main(void)
{
    tc_begin(); p_fpreset = (fpreset_t)tc_crt_proc("_fpreset");
    TC_RUN(exported); TC_RUN(rc_near_after_reset); TC_RUN(em_masked_after_reset);
    return tc_end();
}
