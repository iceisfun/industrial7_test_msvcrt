/* TC
 * id:    fpe_feclearexcept_subset
 * area:  fpenv
 * funcs: _clearfp
 * procs: feraiseexcept feclearexcept fetestexcept
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.6.2.1, 7.6.2.5
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

TC_CASE(clear_inexact_only)
{
    if (!p_feraiseexcept || !p_feclearexcept || !p_fetestexcept) TC_SKIP("missing");
    _clearfp();
    CHECK_EQ_I(p_feraiseexcept(FE_ALL_EXCEPT), 0);
    CHECK_EQ_I(p_feclearexcept(FE_INEXACT), 0);
    CHECK_EQ_I(p_fetestexcept(FE_INEXACT), 0);
    CHECK_EQ_I(p_fetestexcept(FE_ALL_EXCEPT), FE_ALL_EXCEPT & ~FE_INEXACT);
    _clearfp();
}
TC_CASE(clear_all)
{
    if (!p_feraiseexcept || !p_feclearexcept || !p_fetestexcept) TC_SKIP("missing");
    _clearfp();
    p_feraiseexcept(FE_ALL_EXCEPT);
    CHECK_EQ_I(p_feclearexcept(FE_ALL_EXCEPT), 0);
    CHECK_EQ_I(p_fetestexcept(FE_ALL_EXCEPT), 0);
}
TC_CASE(clear_zero_noop)
{
    if (!p_feraiseexcept || !p_feclearexcept || !p_fetestexcept) TC_SKIP("missing");
    _clearfp();
    p_feraiseexcept(FE_OVERFLOW | FE_INVALID);
    CHECK_EQ_I(p_feclearexcept(0), 0);
    /* C11 7.6.2.3 allows raising overflow to also raise inexact, so ignore FE_INEXACT */
    CHECK_EQ_I(p_fetestexcept(FE_ALL_EXCEPT & ~FE_INEXACT), FE_OVERFLOW | FE_INVALID);
    _clearfp();
}
int main(void) { tc_begin(); load_fe(); TC_RUN(clear_inexact_only); TC_RUN(clear_all); TC_RUN(clear_zero_noop); return tc_end(); }
