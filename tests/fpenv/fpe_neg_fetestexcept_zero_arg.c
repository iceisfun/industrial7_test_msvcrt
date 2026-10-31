/* TC
 * id:    fpe_neg_fetestexcept_zero_arg
 * area:  fpenv
 * kind:  negative
 * funcs: _clearfp
 * procs: fetestexcept feraiseexcept feclearexcept
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.6.2.2, 7.6.2.5 (fetestexcept returns the bitwise OR of the requested flags that are set; empty request tests nothing)
 * note:  With excepts == 0, fetestexcept returns 0 and feclearexcept clears nothing, even when flags are pending.
 */
#include "tc.h"
#include <float.h>
/* Microsoft fenv.h values: FE_* exception macros equal the _EM_* values */
#define FE_INEXACT _EM_INEXACT
#define FE_UNDERFLOW _EM_UNDERFLOW
#define FE_OVERFLOW _EM_OVERFLOW
#define FE_DIVBYZERO _EM_ZERODIVIDE
#define FE_INVALID _EM_INVALID
#define FE_ALL_EXCEPT (FE_INEXACT|FE_UNDERFLOW|FE_OVERFLOW|FE_DIVBYZERO|FE_INVALID)
typedef int (__cdecl *ifn_i)(int);
static ifn_i p_test, p_raise, p_clear;
static void load_fe(void) { p_test = (ifn_i)tc_crt_proc("fetestexcept"); p_raise = (ifn_i)tc_crt_proc("feraiseexcept"); p_clear = (ifn_i)tc_crt_proc("feclearexcept"); }
static volatile double v_zero = 0.0, v_one = 1.0, v_max = 1.7976931348623157e308, v_tiny = 1e-300, v_sink;

TC_CASE(zero_arg)
{
    if (!p_test || !p_raise || !p_clear) TC_SKIP("fenv not exported");
    _clearfp();
    CHECK_EQ_I(p_raise(FE_DIVBYZERO | FE_INEXACT | FE_OVERFLOW), 0);
    CHECK_EQ_I(p_test(0), 0);
    CHECK_EQ_I(p_clear(0), 0);
    CHECK_EQ_I(p_test(FE_ALL_EXCEPT), FE_DIVBYZERO | FE_INEXACT | FE_OVERFLOW);
    CHECK_EQ_I(p_clear(FE_ALL_EXCEPT), 0);        /* control */
    CHECK_EQ_I(p_test(FE_ALL_EXCEPT), 0);
    _clearfp();
}
int main(void) { tc_begin(); load_fe(); TC_RUN(zero_arg); return tc_end(); }
