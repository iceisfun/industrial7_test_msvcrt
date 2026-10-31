/* TC
 * id:    fpe_neg_fegetenv_no_side_effects
 * area:  fpenv
 * kind:  negative
 * funcs: _clearfp _controlfp
 * procs: feclearexcept fetestexcept fegetround fesetround fegetenv fesetenv feholdexcept fegetexceptflag fesetexceptflag
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.6.4.1 fegetenv (stores, does not alter); 7.6.4.2 feholdexcept clears flags; MS docs fegetenv
 * note:  fegetenv only reads state; feholdexcept (contrast) clears flags and masks all exceptions.
 */
#include "tc.h"
#include <float.h>

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
#define ALLRC (_MCW_EM | _MCW_RC | _MCW_DN)
typedef struct { unsigned long w[4]; } tc_fenv;      /* fenv_t is two unsigned longs; padded */
typedef struct { unsigned long w[2]; } tc_fexcept;   /* fexcept_t is an unsigned long; padded */
TC_DECL_PROC(int, feclearexcept, (int));
TC_DECL_PROC(int, fetestexcept, (int));
TC_DECL_PROC(int, fegetround, (void));
TC_DECL_PROC(int, fesetround, (int));
TC_DECL_PROC(int, fegetenv, (tc_fenv *));
TC_DECL_PROC(int, fesetenv, (const tc_fenv *));
TC_DECL_PROC(int, feholdexcept, (tc_fenv *));
TC_DECL_PROC(int, fegetexceptflag, (tc_fexcept *, int));
TC_DECL_PROC(int, fesetexceptflag, (const tc_fexcept *, int));
static volatile double v_zero = 0.0, v_one = 1.0, v_three = 3.0, v_sink;
static int load_fe(void)
{
    TC_LOAD_PROC(feclearexcept); TC_LOAD_PROC(fetestexcept);
    TC_LOAD_PROC(fegetround); TC_LOAD_PROC(fesetround); TC_LOAD_PROC(fegetenv); TC_LOAD_PROC(fesetenv);
    TC_LOAD_PROC(feholdexcept); TC_LOAD_PROC(fegetexceptflag); TC_LOAD_PROC(fesetexceptflag);
    return tc_p_feclearexcept && tc_p_fetestexcept && tc_p_fegetround && tc_p_fesetround &&
           tc_p_fegetenv && tc_p_fesetenv && tc_p_feholdexcept && tc_p_fegetexceptflag && tc_p_fesetexceptflag;
}
static int fe_ok;
#define NEED_FE() do { if (!fe_ok) TC_SKIP("fenv functions not exported"); } while (0)
static void logx(const char *what, unsigned long long v) { char b[32]; tc_hex(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write("="); tc_write(b); tc_write("\n"); }

TC_CASE(fegetenv_pure)
{
    tc_fenv e, e2; unsigned orig, ctl; int st;
    NEED_FE(); _clearfp();
    orig = _controlfp(0, 0);
    _controlfp(_RC_UP, _MCW_RC);
    _clearfp();
    v_sink = v_one / v_three; v_sink = v_one / v_zero;                /* masked: sets inexact + divbyzero flags only */
    ctl = _controlfp(0, 0); st = tc_p_fetestexcept(FEX_ALL);
    CHECK_EQ_I(st, FEX_INEXACT | FEX_DIVBYZERO);
    CHECK_EQ_I(tc_p_fegetenv(&e), 0);
    CHECK_EQ_U(_controlfp(0, 0), ctl);
    CHECK_EQ_I(tc_p_fetestexcept(FEX_ALL), st);
    CHECK_EQ_I(tc_p_fegetenv(&e), 0);                                  /* repeatable */
    CHECK_EQ_I(tc_p_fetestexcept(FEX_ALL), st);
    CHECK_EQ_I(tc_p_feholdexcept(&e2), 0);
    CHECK_EQ_I(tc_p_fetestexcept(FEX_ALL), 0);                         /* contrast: held => cleared */
    _controlfp(orig, ALLRC); _clearfp();
}
int main(void) { tc_begin(); fe_ok = load_fe(); TC_RUN(fegetenv_pure); return tc_end(); }
