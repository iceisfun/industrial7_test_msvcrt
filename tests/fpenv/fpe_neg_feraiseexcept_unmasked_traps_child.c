/* TC
 * id:    fpe_neg_feraiseexcept_unmasked_traps_child
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp _clearfp
 * procs: feraiseexcept fetestexcept
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.6.2.3; MS docs _controlfp (unmasked exception raises an SEH floating-point exception)
 * note:  PROBING that feraiseexcept(FE_DIVBYZERO) with ZERODIVIDE unmasked raises the trap instead of returning (C says
 *        feraiseexcept may raise the exception in a way that traps, so a trap is the expected hardware-level result). Accepted exit:
 *        a STATUS_FLOAT_* code 0xC000008D..0xC0000093. The masked control child is in fpe_neg_feraiseexcept_masked_sets_flag_child.
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

TC_CASE(unmasked_traps)
{
    if (!(p_raise)) TC_SKIP("fenv function not exported");
    DWORD code = 0; int ran = tc_spawn_self("trap", &code, 20000);
    CHECK(ran);
    CHECK(code >= 0xC000008Du && code <= 0xC0000093u);
}
int main(void)
{
    tc_begin(); load_fe();
    if (tc_child_name()) {
        if (!p_raise) return 20;
        _clearfp();
        _controlfp(0, _EM_ZERODIVIDE);
        p_raise(FE_DIVBYZERO);
        return 77;
    }
    TC_RUN(unmasked_traps);
    return tc_end();
}
