/* TC
 * id:    fpe_neg_fesetexceptflag_zero_mask
 * area:  fpenv
 * funcs: _clearfp
 * procs: fegetexceptflag fesetexceptflag fetestexcept feraiseexcept
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.6.2.4 (fesetexceptflag sets only the flags in excepts)
 * note:  fexcept_t is treated as an opaque buffer (zero-initialised, 16 bytes).
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
typedef int (__cdecl *get_t)(void *, int);
typedef int (__cdecl *set_t)(const void *, int);
static ifn_i p_test, p_raise; static get_t p_get; static set_t p_set;
TC_CASE(zero_mask)
{
    unsigned long long saved[2] = { 0, 0 };
    if (!p_test || !p_raise || !p_get || !p_set) TC_SKIP("missing");
    _clearfp();
    p_raise(FE_ALL_EXCEPT);
    CHECK_EQ_I(p_get(saved, FE_ALL_EXCEPT), 0);
    _clearfp();
    CHECK_EQ_I(p_test(FE_ALL_EXCEPT), 0);
    CHECK_EQ_I(p_set(saved, 0), 0);
    CHECK_EQ_I(p_test(FE_ALL_EXCEPT), 0);
    _clearfp();
}
TC_CASE(nonzero_mask_control)
{
    unsigned long long saved[2] = { 0, 0 };
    if (!p_test || !p_raise || !p_get || !p_set) TC_SKIP("missing");
    _clearfp();
    p_raise(FE_ALL_EXCEPT);
    p_get(saved, FE_ALL_EXCEPT);
    _clearfp();
    CHECK_EQ_I(p_set(saved, FE_DIVBYZERO), 0);
    CHECK_EQ_I(p_test(FE_ALL_EXCEPT), FE_DIVBYZERO);
    _clearfp();
}
int main(void)
{
    tc_begin();
    p_test = (ifn_i)tc_crt_proc("fetestexcept"); p_raise = (ifn_i)tc_crt_proc("feraiseexcept");
    p_get = (get_t)tc_crt_proc("fegetexceptflag"); p_set = (set_t)tc_crt_proc("fesetexceptflag");
    TC_RUN(zero_mask); TC_RUN(nonzero_mask_control);
    return tc_end();
}
