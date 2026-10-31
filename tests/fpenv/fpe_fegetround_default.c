/* TC
 * id:    fpe_fegetround_default
 * area:  fpenv
 * funcs: _controlfp
 * procs: fegetround
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.6.3.1; MS docs fegetround
 * note:  
 */
#include "tc.h"
#include <float.h>
#define FE_INVALID 0x10 /* MS fenv.h: FE_* flags are the _EM_* values */
#define FE_DIVBYZERO 0x08
#define FE_OVERFLOW 0x04
#define FE_UNDERFLOW 0x02
#define FE_INEXACT 0x01
#define FE_ALL_EXCEPT 0x1F
#define FE_TONEAREST 0x000
#define FE_DOWNWARD 0x100
#define FE_UPWARD 0x200
#define FE_TOWARDZERO 0x300
typedef int (__cdecl *ifn_i)(int);
typedef int (__cdecl *ifn_v)(void);
static ifn_i p_fesetround, p_feraiseexcept, p_feclearexcept, p_fetestexcept;
static ifn_v p_fegetround, p_fpreset_dummy;
static void load_fe(void)
{
    p_fesetround = (ifn_i)tc_crt_proc("fesetround"); p_feraiseexcept = (ifn_i)tc_crt_proc("feraiseexcept");
    p_feclearexcept = (ifn_i)tc_crt_proc("feclearexcept"); p_fetestexcept = (ifn_i)tc_crt_proc("fetestexcept");
    p_fegetround = (ifn_v)tc_crt_proc("fegetround");
}

TC_CASE(exported) { CHECK(p_fegetround != NULL); }
TC_CASE(default_nearest)
{
    if (!p_fegetround) TC_SKIP("no fegetround");
    CHECK_EQ_I(p_fegetround(), FE_TONEAREST);
}
TC_CASE(tracks_controlfp)
{
    unsigned orig = _controlfp(0, 0);
    if (!p_fegetround) TC_SKIP("no fegetround");
    _controlfp(_RC_DOWN, _MCW_RC); CHECK_EQ_I(p_fegetround(), FE_DOWNWARD);
    _controlfp(_RC_UP, _MCW_RC);   CHECK_EQ_I(p_fegetround(), FE_UPWARD);
    _controlfp(_RC_CHOP, _MCW_RC); CHECK_EQ_I(p_fegetround(), FE_TOWARDZERO);
    _controlfp(_RC_NEAR, _MCW_RC); CHECK_EQ_I(p_fegetround(), FE_TONEAREST);
    _controlfp(orig, _MCW_RC);
}
int main(void) { tc_begin(); load_fe(); TC_RUN(exported); TC_RUN(default_nearest); TC_RUN(tracks_controlfp); return tc_end(); }
