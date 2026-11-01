/* TC
 * id:    tm_neg_probe_gmtime_failure_keeps_buffer
 * area:  time
 * kind:  negative
 * funcs: _gmtime64 _localtime64
 * crt:   >=90
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _gmtime64, _localtime64 (return NULL for out-of-range)
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). a failing _gmtime64(-43201) / _localtime64(-1) returns NULL, errno EINVAL, no handler call, and OVERWRITES the thread's static tm (the pointer from the earlier successful call) with all fields -1 -- it does not keep the old value (the test id is historical). -1 is a valid _gmtime64 input (range starts -43200), hence -43201. TZ=UTC0.
 *        thread's static tm returned by an earlier successful call (tm_year stays 70).
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>

TC_CASE(gmtime)
{
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    volatile __time64_t a = 0, b = -43201; struct tm *p, *q;
#else
    volatile __time64_t a = 0, b = -1; struct tm *p, *q;
#endif
    p = _gmtime64((const __time64_t *)&a); CHECK(p != NULL); if (!p) return;
    errno = 0; q = _gmtime64((const __time64_t *)&b);
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    CHECK(q == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(p->tm_year, -1); CHECK_EQ_I(p->tm_mday, -1);   /* failure fills the thread's tm with -1 */
#else
    CHECK(q == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(p->tm_year, 70); CHECK_EQ_I(p->tm_mday, 1);
#endif
}
TC_CASE(localtime)
{
    volatile __time64_t a = 0, b = -1; struct tm *p, *q;
    _putenv("TZ=UTC0"); _tzset();
    p = _localtime64((const __time64_t *)&a); CHECK(p != NULL); if (!p) return;
    errno = 0; q = _localtime64((const __time64_t *)&b);
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    CHECK(q == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(p->tm_year, -1); CHECK_EQ_I(p->tm_mday, -1);   /* failure fills the thread's tm with -1 */
#else
    CHECK(q == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(p->tm_year, 70); CHECK_EQ_I(p->tm_mday, 1);
#endif
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(gmtime); TC_RUN(localtime); _putenv("TZ="); return tc_end(); }
