/* TC
 * id:    fpe_fesetenv_restores_masks_probe
 * area:  fpenv
 * funcs: _clearfp
 * procs: feraiseexcept feclearexcept fetestexcept fegetround fesetround fegetenv fesetenv feholdexcept feupdateenv fegetexceptflag fesetexceptflag
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.6.4.3 (exception masks are not part of the C fenv, but are part of the MS control word)
 * note:  PROBE: fesetenv restores the exception-mask bits saved by fegetenv, and feholdexcept leaves all five masked until fesetenv. Windows fenv_t stores the control word, so likely, but not documented.
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

TC_CASE(masks_restored)
{
    tc_fenv env; unsigned int em0, em_unmasked, em_back;
    NEED_FE(); _clearfp();
    em0 = _controlfp(0, 0) & _MCW_EM;
    tc_p_fegetenv(&env);
    _controlfp(0, _EM_INEXACT);   /* unmask inexact; no FP ops until restored */
    em_unmasked = _controlfp(0, 0) & _EM_INEXACT;
    CHECK_EQ_I(tc_p_fesetenv(&env), 0);
    em_back = _controlfp(0, 0) & _MCW_EM;
    _controlfp(em0, _MCW_EM); _clearfp();
    CHECK_EQ_U(em_unmasked, 0);
    CHECK_EQ_U(em_back, em0);
}
TC_CASE(hold_restores_masks)
{
    tc_fenv env; unsigned int em0, em_back;
    NEED_FE(); _clearfp();
    em0 = _controlfp(0, 0) & _MCW_EM;
    _controlfp(0, _EM_INEXACT | _EM_UNDERFLOW);
    CHECK_EQ_I(tc_p_feholdexcept(&env), 0);
    CHECK_EQ_I(tc_p_fesetenv(&env), 0);
    em_back = _controlfp(0, 0) & 0x1f;
    _controlfp(em0, _MCW_EM); _clearfp();
    CHECK_EQ_U(em_back, 0x1fu & ~(unsigned)(_EM_INEXACT | _EM_UNDERFLOW));
}

int main(void)
{
    tc_begin();
    fe_ok = load_fe();
    TC_RUN(masks_restored); TC_RUN(hold_restores_masks);
    return tc_end();
}
