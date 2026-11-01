/* TC
 * id:    tm_neg_gmtime64_out_of_range
 * area:  time
 * funcs: _gmtime64
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _gmtime64: returns NULL if time before epoch or after 23:59:59 Dec 31 3000
 * note:  ucrtbase accepts [-43200, 32536850399]; negative_time uses -43201 and beyond_max 32536850400 there.
 */
#include "tc.h"

/* Observed limits of the real Windows 10 ucrtbase (build 19045): _MAX__TIME64_T = 32536799999 (3001-01-19 07:59:59 UTC) for local-time style
 * conversions (_localtime64, _ctime64*, _wctime64*, _mktime64, _mkgmtime64, _futime64); _gmtime64 accepts [-43200, 32536850399].
 * The MS docs say 3000-12-31 23:59:59 UTC / 1970-01-01; Windows is more permissive. Older CRTs keep the documented-era values (unverified). */
#if TC_CRT_VER >= 140
#define T64_LOCAL_BAD 32536800000LL
#define T64_GM_BAD    32536850400LL
#define T64_GM_NEG_BAD (-43201LL)
#else
#define T64_LOCAL_BAD 32535244800LL
#define T64_GM_BAD    32535244800LL
#define T64_GM_NEG_BAD (-1LL)
#endif
#include <time.h>
#include <stdlib.h>
#include <errno.h>
static __time64_t g(__time64_t v) { volatile __time64_t x = v; return x; }

TC_CASE(negative_time) { __time64_t t = g(T64_GM_NEG_BAD); struct tm *p; errno = 0; p = _gmtime64(&t); CHECK(p == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(beyond_max) { __time64_t t = g(T64_GM_BAD); struct tm *p; errno = 0; p = _gmtime64(&t); CHECK(p == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(very_negative) { __time64_t t = g(-86400LL * 365 * 100); struct tm *p; errno = 0; p = _gmtime64(&t); CHECK(p == NULL); }
TC_CASE(very_large) { __time64_t t = g(0x7fffffffffffffffLL); struct tm *p; errno = 0; p = _gmtime64(&t); CHECK(p == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(recovers) { __time64_t t = g(0); struct tm *p; p = _gmtime64(&t); CHECK(p != NULL); if (p) CHECK_EQ_I(p->tm_year, 70); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(negative_time); TC_RUN(beyond_max); TC_RUN(very_negative); TC_RUN(very_large); TC_RUN(recovers); return tc_end(); }
