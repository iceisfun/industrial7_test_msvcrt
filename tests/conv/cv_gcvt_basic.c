/* TC
 * id:    cv_gcvt_basic
 * area:  conv
 * funcs: _gcvt
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _gcvt
 * note:  Zero, exponent forms and trailing-dot forms are in cv_gcvt_format_probe.
 */
#include "tc.h"
#include <stdlib.h>

static volatile double v_d;

TC_CASE(pi) { char b[32]; v_d = 3.14159; _gcvt(v_d, 5, b); CHECK_STR(b, "3.1416"); }
TC_CASE(negative) { char b[32]; v_d = -1.5; _gcvt(v_d, 5, b); CHECK_STR(b, "-1.5"); }
TC_CASE(returns_buf) { char b[32]; v_d = 2.5; CHECK(_gcvt(v_d, 5, b) == b); CHECK_STR(b, "2.5"); }
TC_CASE(fraction) { char b[32]; v_d = 0.25; _gcvt(v_d, 5, b); CHECK_STR(b, "0.25"); }

int main(void)
{
    tc_begin();
    TC_RUN(pi); TC_RUN(negative); TC_RUN(returns_buf); TC_RUN(fraction);
    return tc_end();
}
