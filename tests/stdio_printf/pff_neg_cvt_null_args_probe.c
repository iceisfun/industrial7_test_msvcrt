/* TC
 * id:    pff_neg_cvt_null_args_probe
 * area:  stdio_printf
 * funcs: _ecvt _fcvt _gcvt
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _ecvt_s/_fcvt_s/_gcvt_s validate arguments; the non-_s forms are thin wrappers
 * note:  PROBE: NULL decpt / sign / buffer pointer makes _ecvt/_fcvt/_gcvt call the invalid parameter handler once and return NULL with errno EINVAL.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static int p_iph;
TC_CASE(ecvt_null_decpt) { volatile double v = 1.5; int s = 0; char *r; if (!p_iph) TC_SKIP("no iph"); tc_iph_count = 0; errno = 0;
    r = _ecvt(v, 3, NULL, &s); CHECK(r == NULL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(fcvt_null_sign) { volatile double v = 1.5; int d = 0; char *r; if (!p_iph) TC_SKIP("no iph"); tc_iph_count = 0; errno = 0;
    r = _fcvt(v, 3, &d, NULL); CHECK(r == NULL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(gcvt_null_buf) { volatile double v = 1.5; char *r; if (!p_iph) TC_SKIP("no iph"); tc_iph_count = 0; errno = 0;
    r = _gcvt(v, 6, NULL); CHECK(r == NULL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
int main(void) { tc_begin(); p_iph = tc_iph_install(); TC_RUN(ecvt_null_decpt); TC_RUN(fcvt_null_sign); TC_RUN(gcvt_null_buf); return tc_end(); }
