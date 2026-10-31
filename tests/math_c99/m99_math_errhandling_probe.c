/* TC
 * id:    m99_math_errhandling_probe
 * area:  math_c99
 * funcs: log2
 * procs: _statusfp _clearfp
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.12 p9 math_errhandling; MS docs math_errhandling
 * note:  PROBE: what error-reporting channels log2(-1) uses at run time. The MinGW header does not
 *        define math_errhandling, so the MS header's claimed value cannot be compared here; instead
 *        each channel is probed independently: NaN result (C11 F.10.3.10), errno==EDOM (C11 7.12.1
 *        MATH_ERRNO style; MS UCRT docs describe EDOM for log-family), FP invalid flag in _statusfp
 *        (Annex F). The errno and flag cases are independent guesses; msvcr120 is least certain.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

typedef unsigned int (__cdecl *fpfn_t)(void);
static volatile double v_m1 = -1.0;

TC_CASE(nan_result)
{
    volatile double r = log2(v_m1);
    CHECK(r != r);
}
TC_CASE(errno_edom)
{
    volatile double r;
    errno = 0; r = log2(v_m1); CHECK(r != r);
    CHECK_EQ_I(errno, EDOM);
}
TC_CASE(invalid_flag)
{
    fpfn_t st = (fpfn_t)tc_crt_proc("_statusfp"), cl = (fpfn_t)tc_crt_proc("_clearfp");
    volatile double r;
    if (!st || !cl) TC_SKIP("no _statusfp");
    cl(); r = log2(v_m1); CHECK(r != r);
    CHECK((st() & 0x10u) != 0); /* _SW_INVALID */
    cl();
}
int main(void) { tc_begin(); TC_RUN(nan_result); TC_RUN(errno_edom); TC_RUN(invalid_flag); return tc_end(); }
