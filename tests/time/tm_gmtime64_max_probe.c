/* TC
 * id:    tm_gmtime64_max_probe
 * area:  time
 * funcs: _gmtime64
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   CRT source constant _MAX__TIME64_T = 0x793406fff (MS docs describe the limit as 23:59:59 Dec 31 3000)
 * note:  PROBE: the documented limit (3000-12-31 23:59:59 UTC = 32535215999) and the constant 0x793406fff (32535244799,
 *        which is 3001-01-01 07:59:59 UTC) disagree. We believe the CRT compares against 0x793406fff, so 32535216000..32535244799
 *        are accepted and 32535244800 is rejected. Wine behaves this way; real Windows decides.
 */
#include "tc.h"
#include <time.h>

static __time64_t g(__time64_t v) { volatile __time64_t x = v; return x; }

TC_CASE(just_after_documented_limit) { __time64_t t = g(32535216000LL); struct tm *p = _gmtime64(&t);
    CHECK(p != NULL); if (p) { CHECK_EQ_I(p->tm_year, 1101); CHECK_EQ_I(p->tm_mon, 0); CHECK_EQ_I(p->tm_mday, 1); CHECK_EQ_I(p->tm_hour, 0); } }
TC_CASE(constant_max) { __time64_t t = g(0x793406fffLL); struct tm *p = _gmtime64(&t);
    CHECK(p != NULL); if (p) { CHECK_EQ_I(p->tm_year, 1101); CHECK_EQ_I(p->tm_hour, 7); CHECK_EQ_I(p->tm_min, 59); CHECK_EQ_I(p->tm_sec, 59); } }
TC_CASE(constant_plus_one) { __time64_t t = g(0x793407000LL); CHECK(_gmtime64(&t) == NULL); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(just_after_documented_limit); TC_RUN(constant_max); TC_RUN(constant_plus_one); return tc_end(); }
