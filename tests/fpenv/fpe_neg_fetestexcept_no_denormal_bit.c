/* TC
 * id:    fpe_neg_fetestexcept_no_denormal_bit
 * area:  fpenv
 * kind:  negative
 * funcs: _clearfp _statusfp
 * procs: fetestexcept
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.6.2.5 (result is the OR of requested exception macros that are set, so only FE_* bits can appear); MS docs _statusfp (_SW_DENORMAL 0x80000)
 * note:  PROBING that MS fetestexcept masks its result to FE_ALL_EXCEPT and does not leak _SW_DENORMAL (0x80000), even when
 *        _statusfp itself reports a denormal-operand flag (not asserted: x87/SSE dependent).
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

TC_CASE(no_leak)
{
    volatile double d = 4.9406564584124654e-324;   /* denormal */
    unsigned int st; int r;
    if (!p_test) TC_SKIP("fetestexcept not exported");
    _clearfp();
    v_sink = d + v_zero;           /* denormal operand */
    v_sink = d * 0.5;              /* underflow / inexact */
    st = _statusfp();
    (void)st;
    r = p_test(~0);
    CHECK_EQ_I(r & ~FE_ALL_EXCEPT, 0);
    CHECK_EQ_I(p_test(0x80000), 0);
    CHECK_EQ_I(p_test(0x100), 0);
    _clearfp();
}
int main(void) { tc_begin(); load_fe(); TC_RUN(no_leak); return tc_end(); }
