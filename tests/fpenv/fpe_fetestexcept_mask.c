/* TC
 * id:    fpe_fetestexcept_mask
 * area:  fpenv
 * funcs: _clearfp
 * procs: feraiseexcept feclearexcept fetestexcept fegetround fesetround fegetenv fesetenv feholdexcept feupdateenv fegetexceptflag fesetexceptflag
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.6.2.5, 7.6.2.3; MS docs fetestexcept, feraiseexcept
 */
#include "tc.h"
#include <float.h>
#include <errno.h>

/* UCRT <fenv.h> values written out literally: MS values differ from other vendors' headers. */
#define FEX_INEXACT   0x01
#define FEX_UNDERFLOW 0x02
#define FEX_OVERFLOW  0x04
#define FEX_DIVBYZERO 0x08
#define FEX_INVALID   0x10
#define FEX_ALL       0x1f
#define FER_NEAREST   0x000
#define FER_DOWNWARD  0x100
#define FER_UPWARD    0x200
#define FER_TOWARDZERO 0x300
typedef struct { unsigned long w[4]; } tc_fenv;      /* fenv_t is two unsigned longs; padded */
typedef struct { unsigned long w[2]; } tc_fexcept;   /* fexcept_t is an unsigned long; padded */
TC_DECL_PROC(int, feraiseexcept, (int));
TC_DECL_PROC(int, feclearexcept, (int));
TC_DECL_PROC(int, fetestexcept, (int));
TC_DECL_PROC(int, fegetround, (void));
TC_DECL_PROC(int, fesetround, (int));
TC_DECL_PROC(int, fegetenv, (tc_fenv *));
TC_DECL_PROC(int, fesetenv, (const tc_fenv *));
TC_DECL_PROC(int, feholdexcept, (tc_fenv *));
TC_DECL_PROC(int, feupdateenv, (const tc_fenv *));
TC_DECL_PROC(int, fegetexceptflag, (tc_fexcept *, int));
TC_DECL_PROC(int, fesetexceptflag, (const tc_fexcept *, int));
static volatile double v_zero = 0.0, v_one = 1.0, v_max = DBL_MAX, v_sink;
static int load_fe(void)
{
    TC_LOAD_PROC(feraiseexcept); TC_LOAD_PROC(feclearexcept); TC_LOAD_PROC(fetestexcept);
    TC_LOAD_PROC(fegetround); TC_LOAD_PROC(fesetround); TC_LOAD_PROC(fegetenv); TC_LOAD_PROC(fesetenv);
    TC_LOAD_PROC(feholdexcept); TC_LOAD_PROC(feupdateenv); TC_LOAD_PROC(fegetexceptflag); TC_LOAD_PROC(fesetexceptflag);
    return tc_p_feraiseexcept && tc_p_feclearexcept && tc_p_fetestexcept && tc_p_fegetround && tc_p_fesetround &&
           tc_p_fegetenv && tc_p_fesetenv && tc_p_feholdexcept && tc_p_feupdateenv && tc_p_fegetexceptflag && tc_p_fesetexceptflag;
}
static int fe_ok;
#define NEED_FE() do { if (!fe_ok) TC_SKIP("fenv functions not exported"); } while (0)

TC_CASE(subset)
{
    NEED_FE(); _clearfp();
    CHECK_EQ_I(tc_p_feraiseexcept(FEX_DIVBYZERO | FEX_INEXACT), 0);
    CHECK_EQ_I(tc_p_fetestexcept(FEX_DIVBYZERO), FEX_DIVBYZERO);
    CHECK_EQ_I(tc_p_fetestexcept(FEX_INEXACT), FEX_INEXACT);
    CHECK_EQ_I(tc_p_fetestexcept(FEX_OVERFLOW), 0);
    CHECK_EQ_I(tc_p_fetestexcept(FEX_INVALID | FEX_UNDERFLOW), 0);
    _clearfp();
}
TC_CASE(zero_mask) { NEED_FE(); _clearfp(); tc_p_feraiseexcept(FEX_OVERFLOW); CHECK_EQ_I(tc_p_fetestexcept(0), 0); _clearfp(); }
TC_CASE(all_mask)
{
    NEED_FE(); _clearfp();
    tc_p_feraiseexcept(FEX_DIVBYZERO | FEX_INEXACT);
    CHECK_EQ_I(tc_p_fetestexcept(FEX_ALL), FEX_DIVBYZERO | FEX_INEXACT);
    _clearfp();
    CHECK_EQ_I(tc_p_fetestexcept(FEX_ALL), 0);
}

int main(void)
{
    tc_begin();
    fe_ok = load_fe();
    TC_RUN(subset); TC_RUN(zero_mask); TC_RUN(all_mask);
    return tc_end();
}
