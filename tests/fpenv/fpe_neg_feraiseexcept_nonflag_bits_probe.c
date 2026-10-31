/* TC
 * id:    fpe_neg_feraiseexcept_nonflag_bits_probe
 * area:  fpenv
 * kind:  negative
 * funcs: _clearfp
 * procs: feraiseexcept fetestexcept feclearexcept
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.6.2.3 (argument shall be exception macros; others outside contract)
 * note:  PROBING feraiseexcept with non-FE bits (0x80000, 0x100) in a child with a returning handler: must not crash or hang, may
 *        or may not call the handler. Child 'mixed' also passes FE_DIVBYZERO: raised flags must be a subset of FE_DIVBYZERO.
 *        Child 'bad' passes only invalid bits: no flags may appear. Child exit: 77 ok, 78 flags beyond request.
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

TC_CASE(mixed_child)
{
    DWORD code = 0; int ran;
    if (!p_raise || !p_test) TC_SKIP("fenv not exported");
    ran = tc_spawn_self("mixed", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
TC_CASE(bad_child)
{
    DWORD code = 0; int ran;
    if (!p_raise || !p_test) TC_SKIP("fenv not exported");
    ran = tc_spawn_self("bad", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin(); load_fe();
    if ((c = tc_child_name()) != NULL) {
        int r, f; volatile int bits;
        if (!p_raise || !p_test) return 20;
        tc_iph_install(); tc_iph_count = 0;
        _clearfp();
        bits = (c[0] == 'm') ? (0x80000 | 0x100 | FE_DIVBYZERO) : (0x80000 | 0x100);
        r = p_raise(bits); (void)r;
        f = p_test(FE_ALL_EXCEPT);
        if (c[0] == 'm') return (f & ~FE_DIVBYZERO) ? 78 : 77;
        return f ? 78 : 77;
    }
    TC_RUN(mixed_child); TC_RUN(bad_child);
    return tc_end();
}
