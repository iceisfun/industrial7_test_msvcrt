/* TC
 * id:    fpe_neg_fesetround_extreme_values
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp
 * procs: feclearexcept fetestexcept fegetround fesetround fegetenv fesetenv feholdexcept fegetexceptflag fesetexceptflag
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.6.3.2; MS docs fesetround
 * note:  fesetround returns nonzero for anything that is not a rounding macro and must leave the whole control word (EM, RC, DN) untouched.
 *        (The spec value 0x300|0x100 equals FE_TOWARDZERO, so 0x400|0x100 is used instead.)
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

static const int bad[] = { -1, (int)0x80000000, 0x1000, 0x400, 0x500, 0x301, 0x7fffffff };
TC_CASE(rejected_word_identical)
{
    unsigned i, before;
    NEED_FE(); tc_iph_install();
    for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
        before = _controlfp(0, 0);
        CHECK(tc_p_fesetround(bad[i]) != 0);
        CHECK_EQ_U(_controlfp(0, 0), before);
    }
}
TC_CASE(getround_unchanged_per_mode)
{
    static const int ok[] = { FER_NEAREST, FER_DOWNWARD, FER_UPWARD, FER_TOWARDZERO };
    unsigned i, j;
    NEED_FE(); tc_iph_install();
    for (i = 0; i < 4; i++) {
        CHECK_EQ_I(tc_p_fesetround(ok[i]), 0);
        for (j = 0; j < sizeof bad / sizeof bad[0]; j++) {
            CHECK(tc_p_fesetround(bad[j]) != 0);
            CHECK_EQ_I(tc_p_fegetround(), ok[i]);
        }
    }
    tc_p_fesetround(FER_NEAREST);
}
int main(void) { tc_begin(); fe_ok = load_fe(); TC_RUN(rejected_word_identical); TC_RUN(getround_unchanged_per_mode); return tc_end(); }
