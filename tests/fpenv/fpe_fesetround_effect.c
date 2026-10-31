/* TC
 * id:    fpe_fesetround_effect
 * area:  fpenv
 * funcs: _controlfp
 * procs: fesetround
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.6.3.2, F.3; MS docs fesetround
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

static volatile double v_one = 1.0, v_eps = 1e-20, v_three = 3.0;
/* separate negated inputs: GCC (no -frounding-math) may rewrite -a-b as -(a+b), which is only valid in round-to-nearest */
static volatile double v_nega = -1.0;

TC_CASE(upward)
{
    unsigned orig = _controlfp(0, 0); volatile double r, n;
    if (!p_fesetround) TC_SKIP("missing");
    CHECK_EQ_I(p_fesetround(FE_UPWARD), 0);
    r = v_one + v_eps;
    n = v_nega - v_eps;
    CHECK(r > 1.0);
    CHECK(n == -1.0);
    p_fesetround(FE_TONEAREST);
    _controlfp(orig, _MCW_RC);
}
TC_CASE(downward)
{
    unsigned orig = _controlfp(0, 0); volatile double r, n;
    if (!p_fesetround) TC_SKIP("missing");
    CHECK_EQ_I(p_fesetround(FE_DOWNWARD), 0);
    n = v_nega - v_eps;
    r = v_one + v_eps;
    CHECK(n < -1.0);
    CHECK(r == 1.0);
    p_fesetround(FE_TONEAREST);
    _controlfp(orig, _MCW_RC);
}
TC_CASE(towardzero_and_nearest)
{
    unsigned orig = _controlfp(0, 0); volatile double a, b;
    if (!p_fesetround) TC_SKIP("missing");
    CHECK_EQ_I(p_fesetround(FE_TOWARDZERO), 0);
    a = v_one / v_three;                 /* truncated */
    CHECK_EQ_I(p_fesetround(FE_UPWARD), 0);
    b = v_one / v_three;                 /* rounded up */
    CHECK(a < b);
    CHECK_EQ_I(p_fesetround(FE_TONEAREST), 0);
    a = v_one + v_eps;
    CHECK(a == 1.0);
    _controlfp(orig, _MCW_RC);
}
int main(void) { tc_begin(); load_fe(); TC_RUN(upward); TC_RUN(downward); TC_RUN(towardzero_and_nearest); return tc_end(); }
