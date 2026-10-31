/* TC
 * id:    dat_neg_get_tzname_small_buf_probe
 * area:  data_exports
 * funcs: _tzset _putenv _get_tzname
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _get_tzname
 * note:  Probe: a buffer smaller than the name (size 2 for "PST") makes _get_tzname fail with ERANGE and report the required size 4;
 *        a non-NULL buffer with size 0 is an invalid parameter (EINVAL). Whether the invalid-parameter handler runs for ERANGE is not asserted.
 */
#include "tc.h"
#include <errno.h>
#include <stdlib.h>
#include <time.h>

TC_CASE(small_buffer_erange)
{ char b[16] = "keep"; size_t n = 0; int r; _putenv("TZ=PST8PDT"); _tzset();
  r = _get_tzname(&n, b, 2, 0); CHECK_EQ_I(r, ERANGE); CHECK_EQ_U(n, 4);
  _putenv("TZ="); _tzset(); }
TC_CASE(nonnull_buffer_size0_einval)
{ char b[16] = "keep"; size_t n = 0; int r; tc_iph_count = 0; r = _get_tzname(&n, b, 0, 0); CHECK_EQ_I(r, EINVAL); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(small_buffer_erange); TC_RUN(nonnull_buffer_size0_einval); return tc_end(); }
