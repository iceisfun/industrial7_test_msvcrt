/* TC
 * id:    tm_neg_gmtime64_out_of_range
 * area:  time
 * funcs: _gmtime64
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gmtime64: returns NULL if time before epoch or after 23:59:59 Dec 31 3000
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>
static __time64_t g(__time64_t v) { volatile __time64_t x = v; return x; }

TC_CASE(negative_time) { __time64_t t = g(-1); struct tm *p; errno = 0; p = _gmtime64(&t); CHECK(p == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(beyond_max) { __time64_t t = g(32535244800LL); struct tm *p; errno = 0; p = _gmtime64(&t); CHECK(p == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(very_negative) { __time64_t t = g(-86400LL * 365 * 100); struct tm *p; errno = 0; p = _gmtime64(&t); CHECK(p == NULL); }
TC_CASE(very_large) { __time64_t t = g(0x7fffffffffffffffLL); struct tm *p; errno = 0; p = _gmtime64(&t); CHECK(p == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(recovers) { __time64_t t = g(0); struct tm *p; p = _gmtime64(&t); CHECK(p != NULL); if (p) CHECK_EQ_I(p->tm_year, 70); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(negative_time); TC_RUN(beyond_max); TC_RUN(very_negative); TC_RUN(very_large); TC_RUN(recovers); return tc_end(); }
