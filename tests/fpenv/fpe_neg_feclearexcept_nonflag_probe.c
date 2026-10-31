/* TC
 * id:    fpe_neg_feclearexcept_nonflag_probe
 * area:  fpenv
 * funcs: _clearfp
 * procs: feclearexcept fetestexcept feraiseexcept
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.6.2.1 (argument is a bitwise OR of exception macros; other values unspecified)
 * note:  Probe: feclearexcept(0x4000) leaves every flag set and does not call the invalid-parameter handler. The return value is not asserted.
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
static ifn_i p_test, p_raise, p_clear;
TC_CASE(nonflag_clear)
{
    if (!p_test || !p_raise || !p_clear) TC_SKIP("missing");
    _clearfp();
    p_raise(FE_ALL_EXCEPT);
    tc_iph_install(); tc_iph_count = 0;
    (void)p_clear(0x4000);
    CHECK_EQ_I(p_test(FE_ALL_EXCEPT), FE_ALL_EXCEPT);
    CHECK_EQ_I(tc_iph_count, 0);
    _clearfp();
}
int main(void)
{
    tc_begin();
    p_test = (ifn_i)tc_crt_proc("fetestexcept"); p_raise = (ifn_i)tc_crt_proc("feraiseexcept");
    p_clear = (ifn_i)tc_crt_proc("feclearexcept");
    TC_RUN(nonflag_clear);
    return tc_end();
}
