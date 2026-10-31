/* TC
 * id:    fpe_feraiseexcept_each
 * area:  fpenv
 * funcs: _clearfp _statusfp
 * procs: feraiseexcept fetestexcept feclearexcept
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.6.2.3, 7.6.2.5; MS docs feraiseexcept
 * note:  Exceptions stay masked by default, so raising only sets status flags.
 */
#include "tc.h"
#include <float.h>
#ifndef _SW_INEXACT /* mingw float.h lacks the _SW_* aliases of the MS header; values per MS float.h */
#define _SW_INEXACT 0x1
#define _SW_UNDERFLOW 0x2
#define _SW_OVERFLOW 0x4
#define _SW_ZERODIVIDE 0x8
#define _SW_INVALID 0x10
#define _SW_DENORMAL 0x80000
#endif
#define FE_INVALID 0x10 /* MS fenv.h: FE_* flags are the _EM_* values */
#define FE_DIVBYZERO 0x08
#define FE_OVERFLOW 0x04
#define FE_UNDERFLOW 0x02
#define FE_INEXACT 0x01
#define FE_ALL_EXCEPT 0x1F
#define FE_TONEAREST 0x000
#define FE_DOWNWARD 0x100
#define FE_UPWARD 0x200
#define FE_TOWARDZERO 0x300
typedef int (__cdecl *ifn_i)(int);
typedef int (__cdecl *ifn_v)(void);
static ifn_i p_fesetround, p_feraiseexcept, p_feclearexcept, p_fetestexcept;
static ifn_v p_fegetround, p_fpreset_dummy;
static void load_fe(void)
{
    p_fesetround = (ifn_i)tc_crt_proc("fesetround"); p_feraiseexcept = (ifn_i)tc_crt_proc("feraiseexcept");
    p_feclearexcept = (ifn_i)tc_crt_proc("feclearexcept"); p_fetestexcept = (ifn_i)tc_crt_proc("fetestexcept");
    p_fegetround = (ifn_v)tc_crt_proc("fegetround");
}

TC_CASE(each_flag)
{
    static const int fl[5] = { FE_INVALID, FE_DIVBYZERO, FE_OVERFLOW, FE_UNDERFLOW, FE_INEXACT };
    int i;
    if (!p_feraiseexcept || !p_fetestexcept || !p_feclearexcept) TC_SKIP("missing");
    for (i = 0; i < 5; i++) {
        _clearfp();
        CHECK_EQ_I(p_feraiseexcept(fl[i]), 0);
        CHECK((p_fetestexcept(fl[i]) & fl[i]) == fl[i]);
        CHECK_EQ_I(p_feclearexcept(fl[i]), 0);
        CHECK_EQ_I(p_fetestexcept(fl[i]), 0);
    }
    _clearfp();
}
TC_CASE(visible_in_statusfp)
{
    if (!p_feraiseexcept) TC_SKIP("missing");
    _clearfp();
    CHECK_EQ_I(p_feraiseexcept(FE_DIVBYZERO), 0);
    CHECK((_statusfp() & _SW_ZERODIVIDE) != 0);
    _clearfp();
    CHECK_EQ_I(p_feraiseexcept(FE_OVERFLOW), 0);
    CHECK((_statusfp() & _SW_OVERFLOW) != 0);
    _clearfp();
}
TC_CASE(all_except)
{
    if (!p_feraiseexcept || !p_fetestexcept || !p_feclearexcept) TC_SKIP("missing");
    _clearfp();
    CHECK_EQ_I(p_feraiseexcept(FE_ALL_EXCEPT), 0);
    CHECK_EQ_I(p_fetestexcept(FE_ALL_EXCEPT), FE_ALL_EXCEPT);
    CHECK_EQ_I(p_feclearexcept(FE_ALL_EXCEPT), 0);
    CHECK_EQ_I(p_fetestexcept(FE_ALL_EXCEPT), 0);
    CHECK_EQ_U(_statusfp() & 0x1F, 0);
    _clearfp();
}
int main(void) { tc_begin(); load_fe(); TC_RUN(each_flag); TC_RUN(visible_in_statusfp); TC_RUN(all_except); return tc_end(); }
