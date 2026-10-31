/* TC
 * id:    tm_neg_probe_gmtime_failure_keeps_buffer
 * area:  time
 * kind:  negative
 * funcs: _gmtime64 _localtime64
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _gmtime64, _localtime64 (return NULL for out-of-range)
 * note:  Probes that a failing _gmtime64(-1)/_localtime64(-1) returns NULL with errno EINVAL and does not overwrite the
 *        thread's static tm returned by an earlier successful call (tm_year stays 70).
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>

TC_CASE(gmtime)
{
    volatile __time64_t a = 0, b = -1; struct tm *p, *q;
    p = _gmtime64((const __time64_t *)&a); CHECK(p != NULL); if (!p) return;
    errno = 0; q = _gmtime64((const __time64_t *)&b);
    CHECK(q == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(p->tm_year, 70); CHECK_EQ_I(p->tm_mday, 1);
}
TC_CASE(localtime)
{
    volatile __time64_t a = 0, b = -1; struct tm *p, *q;
    _putenv("TZ=UTC0"); _tzset();
    p = _localtime64((const __time64_t *)&a); CHECK(p != NULL); if (!p) return;
    errno = 0; q = _localtime64((const __time64_t *)&b);
    CHECK(q == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(p->tm_year, 70); CHECK_EQ_I(p->tm_mday, 1);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(gmtime); TC_RUN(localtime); _putenv("TZ="); return tc_end(); }
