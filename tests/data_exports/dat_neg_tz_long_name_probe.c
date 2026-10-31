/* TC
 * id:    dat_neg_tz_long_name_probe
 * area:  data_exports
 * funcs: _tzset _putenv _get_tzname _get_timezone
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _tzset: TZ std name is three characters
 * note:  PROBE: names longer than 3 are truncated to 3 chars (PACIFIC8 -> PAC); tzname stays NUL terminated and within bounds.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void setz(const char *s) { _putenv(s); _tzset(); }
TC_CASE(pacific_truncated)
{ char b[64]; size_t n = 0; long tz = 0; setz("TZ=PACIFIC8");
  CHECK_EQ_I(_get_tzname(&n, b, sizeof b, 0), 0); CHECK_STR(b, "PAC");
  CHECK_EQ_I(_get_timezone(&tz), 0); }
TC_CASE(long_both_names_bounded)
{ char a[64], b[64]; size_t n = 0, m = 0; setz("TZ=ABCDEFGHIJ5XYZ");
  memset(a, 0x7e, sizeof a); memset(b, 0x7e, sizeof b);
  CHECK_EQ_I(_get_tzname(&n, a, sizeof a, 0), 0); CHECK_EQ_I(_get_tzname(&m, b, sizeof b, 1), 0);
  CHECK(strlen(a) <= 3); CHECK(strlen(b) <= 3); CHECK(n <= 4); CHECK(m <= 4); }
TC_CASE(est5edt_size_query_4)
{ size_t n = 0; setz("TZ=EST5EDT"); CHECK_EQ_I(_get_tzname(&n, NULL, 0, 0), 0); CHECK_EQ_U(n, 4); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(pacific_truncated); TC_RUN(long_both_names_bounded); TC_RUN(est5edt_size_query_4);
    setz("TZ=");
    return tc_end();
}
