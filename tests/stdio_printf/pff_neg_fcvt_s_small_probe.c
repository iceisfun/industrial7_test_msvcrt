/* TC
 * id:    pff_neg_fcvt_s_small_probe
 * area:  stdio_printf
 * funcs: _fcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _fcvt_s
 * note:  msvcrt.dll and msvcr80 excluded (msvcr80 import lib lacks the _s funcs) (no invalid-parameter handler semantics). Probes: a 3-byte buffer for 10 requested digits of 123.456 returns ERANGE (could instead be EINVAL) and
 *        leaves buf[0]==0. Invalid-parameter handler installed so a fatal abort does not occur; its count is not asserted.
 *        Split per function so one misbehaving function cannot mask the others.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(fcvt_small)
{
    char b[3]; int d = 0, s = 0; errno_t r;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    b[0] = 'Q'; b[1] = 'Q'; b[2] = 'Q';
    r = _fcvt_s(b, 3, 123.456, 10, &d, &s);
    CHECK(r != 0); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(b[0], 0);
}

int main(void) { tc_begin(); TC_RUN(fcvt_small); return tc_end(); }
