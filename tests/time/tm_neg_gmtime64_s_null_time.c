/* TC
 * id:    tm_neg_gmtime64_s_null_time
 * area:  time
 * funcs:
 * procs: _gmtime64_s
 * crt:   >=90 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _gmtime64_s error table
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). -1 is a valid time for _gmtime64_s (range starts at -43200); -43201 returns EINVAL. -1 field fill and handler-on-range-error are probed in tm_neg_probe_gmtime_s_error_effects
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

TC_DECL_PROC(int, _gmtime64_s, (struct tm *, const __time64_t *));

static volatile __time64_t v_t;

TC_CASE(null_time)
{
    struct tm tm; int r; memset(&tm, 0, sizeof tm); errno = 0; tc_iph_count = 0;
    r = tc_p__gmtime64_s(&tm, NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(below_min)
#else
TC_CASE(negative_time)
#endif
{
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t = -43201; errno = 0;
#else
    struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t = -1; errno = 0;
#endif
    r = tc_p__gmtime64_s(&tm, (const __time64_t *)&v_t);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL);
}
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(accepted_minus_one) { struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t = -1; errno = 0; r = tc_p__gmtime64_s(&tm, (const __time64_t *)&v_t); CHECK_EQ_I(r, 0); }
TC_CASE(accepted_min) { struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t = -43200; errno = 0; r = tc_p__gmtime64_s(&tm, (const __time64_t *)&v_t); CHECK_EQ_I(r, 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(_gmtime64_s); if (!tc_p__gmtime64_s) { tc_write("TC FAIL " TC_TEST_ID "::exported _gmtime64_s missing\n"); return 1; } CHECK(tc_iph_install()); TC_RUN(null_time); TC_RUN(below_min); TC_RUN(accepted_minus_one); TC_RUN(accepted_min); return tc_end(); }
#else
int main(void) { tc_begin(); TC_LOAD_PROC(_gmtime64_s); if (!tc_p__gmtime64_s) { tc_write("TC FAIL " TC_TEST_ID "::exported _gmtime64_s missing\n"); return 1; } CHECK(tc_iph_install()); TC_RUN(null_time); TC_RUN(negative_time); return tc_end(); }
#endif
