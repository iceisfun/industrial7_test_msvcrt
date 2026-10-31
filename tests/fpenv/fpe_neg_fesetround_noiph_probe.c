/* TC
 * id:    fpe_neg_fesetround_noiph_probe
 * area:  fpenv
 * funcs:
 * procs: fesetround
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.6.3.2
 * note:  Probe: an invalid rounding argument is reported through the return value only; the invalid-parameter handler is not called.
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
TC_CASE(no_iph)
{
    ifn_i f = (ifn_i)tc_crt_proc("fesetround");
    if (!f) TC_SKIP("missing");
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0;
    CHECK(f(0x400) != 0); CHECK(f(-1) != 0); CHECK(f(0x180) != 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(no_iph); return tc_end(); }
