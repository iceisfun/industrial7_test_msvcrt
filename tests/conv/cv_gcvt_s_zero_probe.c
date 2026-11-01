/* TC
 * id:    cv_gcvt_s_zero_probe
 * area:  conv
 * funcs:
 * procs: _gcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _gcvt_s
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice): _gcvt_s(buf,32,0.0,5) returns 0 and writes "0." (not "0"); same trailing-dot form as _gcvt.
 *        The old CRTs keep the original unverified guess "0".
 */
#include "tc.h"

TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));

TC_CASE(zero)
{
    char b[32]; volatile double v = 0.0;
    if (!tc_p__gcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__gcvt_s(b, sizeof b, v, 5), 0);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "0.");   /* observed Win10 22H2 ucrtbase */
#else
    CHECK_STR(b, "0");
#endif
}
int main(void) { tc_begin(); TC_LOAD_PROC(_gcvt_s); TC_RUN(zero); return tc_end(); }
