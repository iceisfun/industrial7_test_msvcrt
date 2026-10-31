/* TC
 * id:    tm_probe_gmtime64_max_boundary
 * area:  time
 * funcs: _gmtime64
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _gmtime64 (says 23:59:59 Dec 31 3000); _MAX__TIME64_T = 32535244799 in CRT headers
 * note:  PROBE: docs state _gmtime64 range ends Dec 31 3000 23:59:59 (32535215999), but the CRT's internal limit is 0x793406FFF
 *        (32535244799, = 3001-01-01 07:59:59 UTC, the localtime limit). Probes whether gmtime64 accepts times between the two.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

static volatile __time64_t v_t;

TC_CASE(just_after_dec31_3000)
{
    struct tm *r; v_t = 32535216000LL; r = _gmtime64((const __time64_t *)&v_t);
    CHECK(r != NULL);
    if (r) { CHECK_EQ_I(r->tm_year, 1101); CHECK_EQ_I(r->tm_mon, 0); CHECK_EQ_I(r->tm_mday, 1); CHECK_EQ_I(r->tm_hour, 0); }
}
TC_CASE(internal_max)
{
    struct tm *r; v_t = 32535244799LL; r = _gmtime64((const __time64_t *)&v_t);
    CHECK(r != NULL);
    if (r) { CHECK_EQ_I(r->tm_year, 1101); CHECK_EQ_I(r->tm_hour, 7); CHECK_EQ_I(r->tm_min, 59); CHECK_EQ_I(r->tm_sec, 59); }
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(just_after_dec31_3000); TC_RUN(internal_max); return tc_end(); }
