/* TC
 * id:    ext_probe_neg_set_error_mode_invalid_iph
 * area:  exit_signal
 * kind:  negative
 * funcs: _set_error_mode _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_error_mode (invalid value: invalid parameter handler invoked, then -1 and errno EINVAL)
 * note:  Probes that an invalid modeval invokes the invalid-parameter handler exactly once and sets errno=EINVAL on every
 *        CRT >=80; documented for the UCRT, not verified for msvcr80-100.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(invalid_values)
{
    static const int bad[] = { 4, -1, 100 };
    int i, q = _REPORT_ERRMODE;
    int before;
    tc_iph_install();
    before = _set_error_mode(q);
    for (i = 0; i < 3; i++) {
        volatile int v = bad[i];
        int n = tc_iph_count, r;
        errno = 0;
        r = _set_error_mode(v);
        CHECK_EQ_I(r, -1);
        CHECK_EQ_I(errno, EINVAL);
        CHECK_EQ_I(tc_iph_count, n + 1);
        CHECK_EQ_I(_set_error_mode(q), before);   /* mode unchanged */
    }
}
int main(void)
{
    tc_begin();
    TC_RUN(invalid_values);
    return tc_end();
}
