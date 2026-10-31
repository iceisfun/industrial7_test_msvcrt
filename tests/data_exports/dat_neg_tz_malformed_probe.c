/* TC
 * id:    dat_neg_tz_malformed_probe
 * area:  data_exports
 * funcs: _tzset _putenv _get_timezone _get_daylight _get_dstbias
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _tzset
 * note:  PROBE: malformed TZ strings must not crash; only invariant ranges asserted. tzname[] access via _get_tzname.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void inv(void)
{
    long bias = 12345; int d = -1; char a[128], b[128]; size_t n = 0, m = 0;
    CHECK_EQ_I(_get_dstbias(&bias), 0); CHECK(bias <= 0 && bias >= -7200);
    CHECK_EQ_I(_get_daylight(&d), 0); CHECK(d == 0 || d == 1);
    memset(a, 'x', sizeof a); memset(b, 'x', sizeof b);
    _get_tzname(&n, a, sizeof a, 0); _get_tzname(&m, b, sizeof b, 1);
    CHECK(memchr(a, 0, sizeof a) && strlen(a) <= 63); CHECK(memchr(b, 0, sizeof b) && strlen(b) <= 63);
}
TC_CASE(no_offset)
{ long tz = -1; int d = -1; _putenv("TZ=ZZZ"); _tzset(); CHECK_EQ_I(_get_timezone(&tz), 0); CHECK_EQ_I(_get_daylight(&d), 0); CHECK(d == 0 || d == 1); inv(); }
TC_CASE(sign_without_digits)
{ int d = -1; _putenv("TZ=XYZ+"); _tzset(); CHECK_EQ_I(_get_daylight(&d), 0); CHECK(d == 0 || d == 1); inv(); }
TC_CASE(sign_without_digits_daylight0)
{ int d = -1; _putenv("TZ=XYZ+"); _tzset(); CHECK_EQ_I(_get_daylight(&d), 0); CHECK_EQ_I(d, 0); }
TC_CASE(huge_name)
{ static char s[420]; memcpy(s, "TZ=", 3); memset(s + 3, 'Q', 400); s[403] = 0; _putenv(s); _tzset(); inv(); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(no_offset); TC_RUN(sign_without_digits); TC_RUN(sign_without_digits_daylight0); TC_RUN(huge_name);
    _putenv("TZ="); _tzset();
    return tc_end();
}
