/* TC
 * id:    fpe_neg_fetestexcept_nonflag_bits
 * area:  fpenv
 * funcs: _clearfp
 * procs: fetestexcept feraiseexcept
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.6.2.5 (argument shall be a bitwise OR of exception macros; other bits are outside the contract)
 * note:  Probe: passing bits that are not FE_* exception macros (0x4000, ~FE_ALL_EXCEPT) is outside the C contract; asserts that
 *        such bits select no flag (result 0) even with every flag set. Believed true for the MS implementation.
 */
#include "tc.h"
#include <float.h>
/* Microsoft fenv.h values: the FE_* exception macros are the _EM_* values */
#define FE_INEXACT _EM_INEXACT
#define FE_UNDERFLOW _EM_UNDERFLOW
#define FE_OVERFLOW _EM_OVERFLOW
#define FE_DIVBYZERO _EM_ZERODIVIDE
#define FE_INVALID _EM_INVALID
#define FE_ALL_EXCEPT (FE_INEXACT|FE_UNDERFLOW|FE_OVERFLOW|FE_DIVBYZERO|FE_INVALID)
#define FE_TONEAREST 0x000
#define FE_DOWNWARD 0x100
#define FE_UPWARD 0x200
#define FE_TOWARDZERO 0x300
typedef int (__cdecl *ifn_i)(int);
typedef int (__cdecl *ifn_v)(void);
static ifn_i p_test, p_raise;
TC_CASE(nonflag_with_all_raised)
{
    if (!p_test || !p_raise) TC_SKIP("missing");
    _clearfp();
    CHECK_EQ_I(p_raise(FE_ALL_EXCEPT), 0);
    CHECK_EQ_I(p_test(FE_ALL_EXCEPT), FE_ALL_EXCEPT);
    CHECK_EQ_I(p_test(0x4000), 0);
    CHECK_EQ_I(p_test(~FE_ALL_EXCEPT), 0);
    _clearfp();
}
TC_CASE(none_set)
{
    if (!p_test) TC_SKIP("missing");
    _clearfp();
    CHECK_EQ_I(p_test(FE_ALL_EXCEPT), 0);
    CHECK_EQ_I(p_test(0), 0);
}
int main(void)
{
    tc_begin();
    p_test = (ifn_i)tc_crt_proc("fetestexcept"); p_raise = (ifn_i)tc_crt_proc("feraiseexcept");
    TC_RUN(nonflag_with_all_raised); TC_RUN(none_set);
    return tc_end();
}
