/* TC
 * id:    cv_neg_fcvt_s_small_probe
 * area:  conv
 * funcs:
 * procs: _fcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _fcvt_s
 * note:  Probes that _fcvt_s(buf size 4, 123.456, count 5) (needs 3+5+1 bytes) fails with nonzero return instead of overrunning. Separate file because it is a hang risk on non-conforming runtimes.
 */
#include "tc.h"

TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));
TC_CASE(fcvt_small_buffer_fails)
{
    char b[4]; int d, s;
    if (!tc_p__fcvt_s) TC_SKIP("not exported");
    CHECK(tc_p__fcvt_s(b, 4, 123.456, 5, &d, &s) != 0);
}

int main(void)
{
    tc_begin(); CHECK(tc_iph_install()); TC_LOAD_PROC(_fcvt_s);
    TC_RUN(fcvt_small_buffer_fails);
    return tc_end();
}
