/* TC
 * id:    ext_neg_set_error_mode_invalid
 * area:  exit_signal
 * kind:  negative
 * funcs: _set_error_mode _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_error_mode: invalid modeval -> returns -1
 * note:  errno/handler assertions are in ext_probe_neg_set_error_mode_invalid_iph (version-dependent for old CRTs).
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
        int r;
        errno = 0;
        r = _set_error_mode(v);
        CHECK_EQ_I(r, -1);
        CHECK_EQ_I(_set_error_mode(q), before);   /* mode unchanged */
    }
}
int main(void)
{
    tc_begin();
    TC_RUN(invalid_values);
    return tc_end();
}
