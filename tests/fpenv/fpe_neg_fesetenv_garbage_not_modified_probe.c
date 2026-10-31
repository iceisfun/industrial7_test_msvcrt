/* TC
 * id:    fpe_neg_fesetenv_garbage_not_modified_probe
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp _clearfp
 * procs: feclearexcept fetestexcept fegetround fesetround fegetenv fesetenv feholdexcept fegetexceptflag fesetexceptflag
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fesetenv; control word layout in float.h
 * note:  PROBE: fesetenv with an all-zero fenv_t (a zero control word would mean everything unmasked on raw hardware). Runs in a child because it may unmask exceptions.
 *        Asserts only that no undefined bits show in _controlfp(0,0), and that restoring the saved env works. Return value of fesetenv(zero) is logged only.
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

static DWORD run(const char *n) { DWORD c = 0; int ran = tc_spawn_self(n, &c, 20000); if (!ran) c = 0xdead0002; logx(n, c); return c; }
TC_CASE(zero_env_child) { CHECK_EQ_U(run("zero"), 0x40); }
int main(void)
{
    const char *c;
    tc_begin(); fe_ok = load_fe();
    if ((c = tc_child_name()) != NULL) {
        tc_fenv saved, zero; unsigned i, w0, w1, w2, code = 0x40;
        if (!fe_ok) return 3;
        for (i = 0; i < 4; i++) zero.w[i] = 0;
        _clearfp();
        w0 = _controlfp(0, 0);
        tc_p_fegetenv(&saved);
        tc_p_fesetenv(&zero);
        w1 = _controlfp(0, 0);
        if (w1 & ~(unsigned)(_MCW_EM | _MCW_RC | _MCW_DN | _MCW_PC | _MCW_IC)) code |= 1;
        if (tc_p_fesetenv(&saved) != 0) code |= 2;
        _clearfp();
        w2 = _controlfp(0, 0);
        if (w2 != w0) code |= 4;
        return (int)code;
    }
    TC_RUN(zero_env_child);
    return tc_end();
}
