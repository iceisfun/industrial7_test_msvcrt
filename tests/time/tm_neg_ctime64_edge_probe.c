/* TC
 * id:    tm_neg_ctime64_edge_probe
 * area:  time
 * kind:  negative
 * funcs: _ctime64
 * crt:   >=90
 * tier:  p2
 * conf:  observed
 * ref:   MS docs ctime64 range
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _ctime64 (TZ=UTC0; identical for PST8PDT/JST-9) accepts up to 32536799999 (3001-01-19 07:59:59 UTC), NOT 3000-12-31 as the docs state; 32536800000 returns NULL. Docs deviation, expectation = Windows.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

static volatile __time64_t vt;

TC_CASE(one_past_max)
{
    __time64_t t; char *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset();
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    vt = 32536800000LL; t = vt; r = _ctime64(&t);
#else
    vt = 32535216000LL; t = vt; r = _ctime64(&t);
#endif
    CHECK(r == NULL);
}
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(last_valid)
#else
TC_CASE(plus_8h)
#endif
{
    __time64_t t; char *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset();
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    vt = 32536799999LL; t = vt; r = _ctime64(&t);
    CHECK(r != NULL);
    vt = 32535244799LL; t = vt; r = _ctime64(&t);   /* the 8h-adjusted limit of older CRTs is accepted here */
    CHECK(r != NULL);
#else
    vt = 32535244799LL; t = vt; r = _ctime64(&t);
    CHECK(r == NULL);
#endif
}
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
int main(void) { tc_begin(); TC_RUN(one_past_max); TC_RUN(last_valid); return tc_end(); }
#else
int main(void) { tc_begin(); TC_RUN(one_past_max); TC_RUN(plus_8h); return tc_end(); }
#endif
