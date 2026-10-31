/* TC
 * id:    dat_neg_get_tzname_badindex_outputs_probe
 * area:  data_exports
 * funcs: _get_tzname
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _get_tzname (EINVAL for bad index/NULL)
 * note:  PROBE: buffer state after failed _get_tzname: secure-CRT convention is that buf[0] is set to 0. Each assertion in its own case. EINVAL/iph checks are in the _iph cases.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <time.h>

TC_CASE(idx2_einval) { char b[8]; size_t n = 99; int r; memcpy(b, "xxxx", 5); tc_iph_count = 0;
  r = _get_tzname(&n, b, sizeof b, 2); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(idx2_buf_emptied) { char b[8]; size_t n = 99; memcpy(b, "xxxx", 5);
  _get_tzname(&n, b, sizeof b, 2); CHECK_EQ_I(b[0], 0); }
TC_CASE(idxm1_einval) { char b[8]; size_t n = 99; int r; memcpy(b, "xxxx", 5); tc_iph_count = 0;
  r = _get_tzname(&n, b, sizeof b, -1); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(idxm1_buf_emptied) { char b[8]; size_t n = 99; memcpy(b, "xxxx", 5);
  _get_tzname(&n, b, sizeof b, -1); CHECK_EQ_I(b[0], 0); }
TC_CASE(null_ret_einval) { char b[8]; int r; size_t *volatile np = NULL; memcpy(b, "xxxx", 5); tc_iph_count = 0;
  r = _get_tzname(np, b, sizeof b, 0); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(idx2_einval); TC_RUN(idx2_buf_emptied); TC_RUN(idxm1_einval); TC_RUN(idxm1_buf_emptied);
    TC_RUN(null_ret_einval);
    return tc_end();
}
