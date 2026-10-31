/* TC
 * id:    fpe_neg_feraiseexcept_masked_sets_flag_child
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp _clearfp
 * procs: feraiseexcept fetestexcept
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.6.2.3, 7.6.2.5
 * note:  Control for fpe_neg_feraiseexcept_unmasked_traps_child: with ZERODIVIDE masked, feraiseexcept returns normally and the
 *        flag is set. Child process isolates any unexpected trap.
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

TC_CASE(masked_ok)
{
    if (!(p_raise && p_test)) TC_SKIP("fenv function not exported");
    DWORD code = 0; int ran = tc_spawn_self("masked", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 77);
}
int main(void)
{
    tc_begin(); load_fe();
    if (tc_child_name()) {
        if (!p_raise || !p_test) return 20;
        _clearfp();
        _controlfp(_EM_ZERODIVIDE, _EM_ZERODIVIDE);
        if (p_raise(FE_DIVBYZERO) != 0) return 21;
        if (!(p_test(FE_DIVBYZERO) & FE_DIVBYZERO)) return 22;
        return 77;
    }
    TC_RUN(masked_ok);
    return tc_end();
}
