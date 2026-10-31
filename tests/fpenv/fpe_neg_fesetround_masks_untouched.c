/* TC
 * id:    fpe_neg_fesetround_masks_untouched
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp _clearfp
 * procs: feclearexcept fetestexcept fegetround fesetround fegetenv fesetenv feholdexcept fegetexceptflag fesetexceptflag
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fesetround, _controlfp (RC field only)
 * note:  Successful fesetround must change only the rounding-control bits. Invalid exception is unmasked while testing; no FP arithmetic is done in that window.
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

TC_CASE(only_rc_changes)
{
    static const int modes[] = { FER_DOWNWARD, FER_UPWARD, FER_TOWARDZERO, FER_NEAREST };
    unsigned orig, base, i, w;
    NEED_FE();
    _clearfp();
    orig = _controlfp(0, 0);
    _controlfp(_MCW_EM, _MCW_EM);
    _controlfp(0, _EM_INVALID);          /* invalid unmasked, others masked */
#ifdef TC_UCRT
    _controlfp(_DN_FLUSH, _MCW_DN);
#endif
    base = _controlfp(0, 0) & ~(unsigned)_MCW_RC;
    logx("base", base);
    for (i = 0; i < 4; i++) {
        CHECK_EQ_I(tc_p_fesetround(modes[i]), 0);
        w = _controlfp(0, 0);
        CHECK_EQ_U(w & ~(unsigned)_MCW_RC, base);
        CHECK_EQ_U(w & _MCW_RC, (unsigned)modes[i]);
    }
    _controlfp(orig, ALLRC);
    CHECK_EQ_U(_controlfp(0, 0) & ALLRC, orig & ALLRC);
}
int main(void) { tc_begin(); fe_ok = load_fe(); TC_RUN(only_rc_changes); return tc_end(); }
