/* TC
 * id:    fpe_fe_constants_values
 * area:  fpenv
 * funcs: _clearfp
 * procs: fegetround fesetround fetestexcept feraiseexcept
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS UCRT <fenv.h>; MS docs fegetround, fesetround
 * note:  Values are the UCRT header's; verified through behaviour (flags raised by arithmetic, rounding via _controlfp) rather than via the build header, which may differ.
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

TC_CASE(exception_values)
{
    NEED_FE(); _clearfp();
    CHECK_EQ_I(tc_p_feraiseexcept(FEX_INEXACT), 0);   CHECK_EQ_I(_statusfp() & 0x1f, 0x01); _clearfp();
    CHECK_EQ_I(tc_p_feraiseexcept(FEX_DIVBYZERO), 0); CHECK_EQ_I(_statusfp() & 0x1f, 0x08); _clearfp();
    CHECK_EQ_I(tc_p_feraiseexcept(FEX_INVALID), 0);   CHECK_EQ_I(_statusfp() & 0x1f, 0x10); _clearfp();
    CHECK_EQ_I(tc_p_feraiseexcept(FEX_OVERFLOW), 0);  CHECK((_statusfp() & 0x04) == 0x04); _clearfp();
    CHECK_EQ_I(tc_p_feraiseexcept(FEX_UNDERFLOW), 0); CHECK((_statusfp() & 0x02) == 0x02); _clearfp();
}
TC_CASE(rounding_values)
{
    unsigned int rc0;
    NEED_FE(); _clearfp(); rc0 = _controlfp(0, 0) & _MCW_RC;
    CHECK_EQ_I(tc_p_fesetround(FER_DOWNWARD), 0);   CHECK_EQ_I(_controlfp(0, 0) & _MCW_RC, _RC_DOWN); CHECK_EQ_I(tc_p_fegetround(), 0x100);
    CHECK_EQ_I(tc_p_fesetround(FER_UPWARD), 0);     CHECK_EQ_I(_controlfp(0, 0) & _MCW_RC, _RC_UP);   CHECK_EQ_I(tc_p_fegetround(), 0x200);
    CHECK_EQ_I(tc_p_fesetround(FER_TOWARDZERO), 0); CHECK_EQ_I(_controlfp(0, 0) & _MCW_RC, _RC_CHOP); CHECK_EQ_I(tc_p_fegetround(), 0x300);
    CHECK_EQ_I(tc_p_fesetround(FER_NEAREST), 0);    CHECK_EQ_I(tc_p_fegetround(), 0);
    _controlfp(rc0, _MCW_RC); _clearfp();
}

int main(void)
{
    tc_begin();
    fe_ok = load_fe();
    TC_RUN(exception_values); TC_RUN(rounding_values);
    return tc_end();
}
