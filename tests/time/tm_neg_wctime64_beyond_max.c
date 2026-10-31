/* TC
 * id:    tm_neg_wctime64_beyond_max
 * area:  time
 * kind:  negative
 * funcs: _wctime64
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs ctime, _ctime64, _wctime64 (valid through 23:59:59 Dec 31 3000 UTC; NULL otherwise)
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
static volatile __time64_t vt;
static wchar_t *w(__time64_t v) { __time64_t t; _putenv("TZ=UTC0"); _tzset(); vt = v; t = vt; return _wctime64(&t); }
TC_CASE(just_beyond) { CHECK(w(32535216000LL) == NULL); }
TC_CASE(llong_max) { CHECK(w(LLONG_MAX) == NULL); }
TC_CASE(llong_min) { CHECK(w(LLONG_MIN) == NULL); }
TC_CASE(last_valid) { wchar_t *r = w(32535215999LL); CHECK(r != NULL); if (r) CHECK_WSTR(r, L"Wed Dec 31 23:59:59 3000\n"); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(just_beyond); TC_RUN(llong_max); TC_RUN(llong_min); TC_RUN(last_valid); return tc_end(); }
