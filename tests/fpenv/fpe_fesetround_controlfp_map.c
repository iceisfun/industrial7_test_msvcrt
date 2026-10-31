/* TC
 * id:    fpe_fesetround_controlfp_map
 * area:  fpenv
 * funcs: _controlfp
 * procs: fesetround fegetround
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs fesetround, _controlfp; fenv.h FE_* rounding macros = _RC_* values
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

TC_CASE(fe_to_rc)
{
    unsigned orig = _controlfp(0, 0);
    if (!p_fesetround) TC_SKIP("missing");
    CHECK_EQ_I(p_fesetround(FE_UPWARD), 0);     CHECK_EQ_U(_controlfp(0, 0) & _MCW_RC, _RC_UP);
    CHECK_EQ_I(p_fesetround(FE_DOWNWARD), 0);   CHECK_EQ_U(_controlfp(0, 0) & _MCW_RC, _RC_DOWN);
    CHECK_EQ_I(p_fesetround(FE_TOWARDZERO), 0); CHECK_EQ_U(_controlfp(0, 0) & _MCW_RC, _RC_CHOP);
    CHECK_EQ_I(p_fesetround(FE_TONEAREST), 0);  CHECK_EQ_U(_controlfp(0, 0) & _MCW_RC, _RC_NEAR);
    _controlfp(orig, _MCW_RC);
}
TC_CASE(rc_to_fe)
{
    unsigned orig = _controlfp(0, 0);
    if (!p_fegetround) TC_SKIP("missing");
    _controlfp(_RC_UP, _MCW_RC);
    CHECK_EQ_I(p_fegetround(), FE_UPWARD);
    _controlfp(orig, _MCW_RC);
}
int main(void) { tc_begin(); load_fe(); TC_RUN(fe_to_rc); TC_RUN(rc_to_fe); return tc_end(); }
