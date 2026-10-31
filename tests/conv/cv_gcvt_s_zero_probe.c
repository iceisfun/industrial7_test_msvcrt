/* TC
 * id:    cv_gcvt_s_zero_probe
 * area:  conv
 * funcs:
 * procs: _gcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _gcvt_s
 * note:  Probes the text _gcvt_s produces for 0.0 with 5 digits: expected "0" (%g style). Older CRTs could emit "0." or "0.0000".
 */
#include "tc.h"

TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));

TC_CASE(zero)
{
    char b[32]; volatile double v = 0.0;
    if (!tc_p__gcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__gcvt_s(b, sizeof b, v, 5), 0);
    CHECK_STR(b, "0");
}
int main(void) { tc_begin(); TC_LOAD_PROC(_gcvt_s); TC_RUN(zero); return tc_end(); }
