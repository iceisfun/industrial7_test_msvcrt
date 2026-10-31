/* TC
 * id:    pff_neg_cvt_null_buffer_outparams_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _ecvt_s _fcvt_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _ecvt_s/_fcvt_s (parameters validated before any output is produced)
 * note:  PROBES: when the buffer is NULL (EINVAL), the decimal-point and sign out-parameters are left unmodified.
 *        Split from pff_neg_ecvt_fcvt_gcvt_null_buffer so a wrong guess cannot contaminate the documented checks.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(ecvt_outparams_untouched) { int d = 77, s = 88; char * volatile b = NULL; errno_t r; tc_iph_count = 0;
    r = _ecvt_s(b, 20, 1.0, 5, &d, &s); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(d, 77); CHECK_EQ_I(s, 88); }
TC_CASE(fcvt_outparams_untouched) { int d = 77, s = 88; char * volatile b = NULL; errno_t r; tc_iph_count = 0;
    r = _fcvt_s(b, 20, -1.0, 2, &d, &s); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(d, 77); CHECK_EQ_I(s, 88); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(ecvt_outparams_untouched); TC_RUN(fcvt_outparams_untouched);
    return tc_end();
}
