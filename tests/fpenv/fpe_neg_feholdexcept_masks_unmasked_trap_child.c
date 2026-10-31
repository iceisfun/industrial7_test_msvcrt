/* TC
 * id:    fpe_neg_feholdexcept_masks_unmasked_trap_child
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp _clearfp
 * procs: feholdexcept fesetenv fetestexcept feraiseexcept
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   C11 7.6.4.2 (feholdexcept saves the environment, clears flags, installs a non-stop mode, if available); MS docs feholdexcept, _controlfp (unmasked exceptions raise SEH float exceptions)
 * note:  After feholdexcept a previously unmasked divide-by-zero must not trap. The control child (no feholdexcept) must die with
 *        STATUS_FLOAT_DIVIDE_BY_ZERO (0xC000008E): that is Windows SEH behaviour (documented), not ISO C, hence conf documented.
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
typedef struct { unsigned long w[4]; } tc_fenv;
typedef int (__cdecl *hold_t)(tc_fenv *);
static hold_t p_hold;

TC_CASE(hold_prevents_trap)
{
    if (!(p_hold)) TC_SKIP("fenv function not exported");
    DWORD code = 0; int ran = tc_spawn_self("hold", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
TC_CASE(without_hold_traps)
{
    if (!(p_hold)) TC_SKIP("fenv function not exported");
    DWORD code = 0; int ran = tc_spawn_self("nohold", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0xC000008Eu);
}
TC_CASE(hold_clears_flags)
{
    tc_fenv env; unsigned int old = _controlfp(0, 0);
    if (!p_hold || !p_raise || !p_test) TC_SKIP("fenv not exported");
    _clearfp();
    CHECK_EQ_I(p_raise(FE_INEXACT | FE_OVERFLOW), 0);
    CHECK_EQ_I(p_hold(&env), 0);
    CHECK_EQ_I(p_test(FE_ALL_EXCEPT), 0);
    _controlfp(old, _MCW_EM | _MCW_RC); _clearfp();
}
int main(void)
{
    const char *c;
    tc_begin(); load_fe(); p_hold = (hold_t)tc_crt_proc("feholdexcept");
    if ((c = tc_child_name()) != NULL) {
        tc_fenv env;
        if (!p_hold) return 20;
        _clearfp();
        _controlfp(0, _EM_ZERODIVIDE);
        if (c[0] == 'h') {
            if (p_hold(&env) != 0) return 21;
            v_sink = v_one / v_zero;
            if (_controlfp(0, 0) & _EM_ZERODIVIDE) return 77;
            return 22;
        }
        v_sink = v_one / v_zero;
        return 77;
    }
    TC_RUN(hold_prevents_trap); TC_RUN(without_hold_traps); TC_RUN(hold_clears_flags);
    return tc_end();
}
