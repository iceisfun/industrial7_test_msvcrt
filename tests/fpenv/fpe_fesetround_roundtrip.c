/* TC
 * id:    fpe_fesetround_roundtrip
 * area:  fpenv
 * funcs: _controlfp
 * procs: fesetround fegetround
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.6.3.2; MS docs fesetround
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

TC_CASE(roundtrip)
{
    static const int modes[4] = { FE_TONEAREST, FE_DOWNWARD, FE_UPWARD, FE_TOWARDZERO };
    unsigned orig = _controlfp(0, 0); int i;
    if (!p_fesetround || !p_fegetround) TC_SKIP("missing");
    for (i = 0; i < 4; i++) {
        CHECK_EQ_I(p_fesetround(modes[i]), 0);
        CHECK_EQ_I(p_fegetround(), modes[i]);
    }
    _controlfp(orig, _MCW_RC);
}
TC_CASE(restore)
{
    unsigned orig = _controlfp(0, 0);
    if (!p_fesetround || !p_fegetround) TC_SKIP("missing");
    CHECK_EQ_I(p_fesetround(FE_UPWARD), 0);
    CHECK_EQ_I(p_fesetround(FE_TONEAREST), 0);
    CHECK_EQ_I(p_fegetround(), FE_TONEAREST);
    CHECK_EQ_U(_controlfp(0, 0) & _MCW_RC, _RC_NEAR);
    _controlfp(orig, _MCW_RC);
}
int main(void) { tc_begin(); load_fe(); TC_RUN(roundtrip); TC_RUN(restore); return tc_end(); }
