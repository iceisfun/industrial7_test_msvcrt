/* TC
 * id:    dat_time_vars_tzname_sizequery_probe
 * area:  data_exports
 * funcs: _tzset _putenv _get_tzname
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _get_tzname
 * note:  Probe: _get_tzname(&n, NULL, 0, idx) succeeds (returns 0, no invalid-parameter call) and reports the size including the NUL
 *        (4 for "PST"/"PDT") without copying anything.
 */
#include "tc.h"
#include <errno.h>
#include <stdlib.h>
#include <time.h>

TC_CASE(null_buffer_size_query)
{
    size_t n = 0, m = 0;
    _putenv("TZ=PST8PDT"); _tzset();
    tc_iph_count = 0;
    CHECK_EQ_I(_get_tzname(&n, NULL, 0, 0), 0); CHECK_EQ_U(n, 4);
    CHECK_EQ_I(_get_tzname(&m, NULL, 0, 1), 0); CHECK_EQ_U(m, 4);
    CHECK_EQ_I(tc_iph_count, 0);
    _putenv("TZ="); _tzset();
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_buffer_size_query); return tc_end(); }
