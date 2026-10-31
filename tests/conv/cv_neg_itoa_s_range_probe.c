/* TC
 * id:    cv_neg_itoa_s_range_probe
 * area:  conv
 * funcs: _itoa_s _i64toa_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _itoa_s (table says EINVAL for a too-small buffer)
 * note:  Probes the exact error code (ERANGE believed, docs say EINVAL), that buf[0] is set to 0 on failure,
 *        and that buf[0] is zeroed for a bad radix. Each in its own case.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static volatile int v_i; static volatile __int64 v_64; static volatile size_t v_n; static volatile int v_radix;

TC_CASE(too_small_code) { char b[8]; v_i = 12345; v_n = 5; tc_iph_count = 0; CHECK_EQ_I(_itoa_s(v_i, b, v_n, 10), ERANGE); }
TC_CASE(too_small_empty) { char b[8]; memset(b, 'x', sizeof b); v_i = 12345; v_n = 5; _itoa_s(v_i, b, v_n, 10); CHECK_EQ_I(b[0], 0); }
TC_CASE(i64_min_size20_code) { char b[32]; v_64 = -9223372036854775807LL - 1; v_n = 20; CHECK_EQ_I(_i64toa_s(v_64, b, v_n, 10), ERANGE); }
TC_CASE(bad_radix_empty) { char b[8]; memset(b, 'x', sizeof b); v_radix = 37; _itoa_s(1, b, 8, v_radix); CHECK_EQ_I(b[0], 0); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no invalid parameter handler API\n"); return tc_end(); }
    TC_RUN(too_small_code); TC_RUN(too_small_empty); TC_RUN(i64_min_size20_code); TC_RUN(bad_radix_empty);
    return tc_end();
}
