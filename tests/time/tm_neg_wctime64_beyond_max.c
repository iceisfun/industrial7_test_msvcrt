/* TC
 * id:    tm_neg_wctime64_beyond_max
 * area:  time
 * kind:  negative
 * funcs: _wctime64
 * crt:   >=90
 * tier:  p0
 * conf:  observed
 * ref:   MS docs ctime, _ctime64, _wctime64 (valid through 23:59:59 Dec 31 3000 UTC; NULL otherwise)
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _wctime64 accepts up to 32536799999 (3001-01-19 07:59:59 UTC), not Dec 31 3000 as docs state; 32536800000, LLONG_MAX, LLONG_MIN return NULL. 'Wed Dec 31 23:59:59 3000' still formats correctly. Docs deviation.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
static volatile __time64_t vt;
static wchar_t *w(__time64_t v) { __time64_t t; _putenv("TZ=UTC0"); _tzset(); vt = v; t = vt; return _wctime64(&t); }
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(just_beyond) { CHECK(w(32536800000LL) == NULL); }
TC_CASE(past_3000_still_valid) { wchar_t *r = w(32535216000LL); CHECK(r != NULL); }
TC_CASE(real_last_valid) { CHECK(w(32536799999LL) != NULL); }
#else
TC_CASE(just_beyond) { CHECK(w(32535216000LL) == NULL); }
#endif
TC_CASE(llong_max) { CHECK(w(LLONG_MAX) == NULL); }
TC_CASE(llong_min) { CHECK(w(LLONG_MIN) == NULL); }
TC_CASE(last_valid) { wchar_t *r = w(32535215999LL); CHECK(r != NULL); if (r) CHECK_WSTR(r, L"Wed Dec 31 23:59:59 3000\n"); }
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(just_beyond); TC_RUN(past_3000_still_valid); TC_RUN(real_last_valid); TC_RUN(llong_max); TC_RUN(llong_min); TC_RUN(last_valid); return tc_end(); }
#else
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(just_beyond); TC_RUN(llong_max); TC_RUN(llong_min); TC_RUN(last_valid); return tc_end(); }
#endif
