/* TC
 * id:    exp_neg_gmtime64_s_negative_probe
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _gmtime64_s
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _gmtime64_s (negative inputs undocumented in older CRTs)
 * note:  PROBE: older CRTs (<140) return EINVAL for time_t -1; UCRT is believed to accept it (1969-12-31 23:59:59, return 0). Per-CRT result is the finding.
 * note:  Class F (harness): TC_UCRT is documented in tc.h but never defined by tools/tc.py, so the UCRT branch was dead code and the
 *        legacy EINVAL expectation was applied to ucrtbase. The UCRT branch (return 0, 1969-12-31 23:59:59) is the pre-existing intended expectation.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

typedef int (__cdecl *gm_t)(struct tm *, const __time64_t *);
static __time64_t m1 = -1;

TC_CASE(minus1_result)
{
    struct tm t; gm_t gm = (gm_t)tc_crt_proc("_gmtime64_s");
    CHECK(gm != NULL); if (!gm) return;
    t.tm_year = 0;
#if defined(TC_UCRT) || defined(_UCRT)   /* tc.py only defines _UCRT for ucrtbase */
    CHECK_EQ_I(gm(&t, &m1), 0); CHECK_EQ_I(t.tm_year, 69); CHECK_EQ_I(t.tm_mon, 11); CHECK_EQ_I(t.tm_mday, 31);
    CHECK_EQ_I(t.tm_hour, 23); CHECK_EQ_I(t.tm_min, 59); CHECK_EQ_I(t.tm_sec, 59);
#else
    CHECK_EQ_I(gm(&t, &m1), EINVAL);
#endif
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(minus1_result); return tc_end(); }
