/* TC
 * id:    tm_neg_gmtime32_s_args
 * area:  time
 * funcs:
 * procs: _gmtime32_s
 * crt:   >=90 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _gmtime32_s
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _gmtime32_s accepts -43200..-1 (returns 0; the 32-bit path truncates toward zero so -1 yields 1970-01-01 00:00 with tm_sec == -1) and rejects -43201 (EINVAL); docs say invalid before Jan 1 1970, Windows deviates. Gated to ucrtbase: older CRTs not yet run.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

TC_DECL_PROC(int, _gmtime32_s, (struct tm *, const __time32_t *));

static volatile __time32_t v_t;

TC_CASE(null_tm) { int r; v_t = 0; errno = 0; r = tc_p__gmtime32_s(NULL, (const __time32_t *)&v_t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(null_time) { struct tm tm; int r; memset(&tm, 0, sizeof tm); errno = 0; r = tc_p__gmtime32_s(&tm, NULL); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(below_min) { struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t = -43201; errno = 0; r = tc_p__gmtime32_s(&tm, (const __time32_t *)&v_t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(accepted_minus_one) { struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t = -1; errno = 0; r = tc_p__gmtime32_s(&tm, (const __time32_t *)&v_t); CHECK_EQ_I(r, 0); CHECK_EQ_I(tm.tm_year, 70); CHECK_EQ_I(tm.tm_mday, 1); CHECK_EQ_I(tm.tm_sec, -1); }
TC_CASE(accepted_min) { struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t = -43200; errno = 0; r = tc_p__gmtime32_s(&tm, (const __time32_t *)&v_t); CHECK_EQ_I(r, 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(_gmtime32_s); if (!tc_p__gmtime32_s) { tc_write("TC FAIL " TC_TEST_ID "::exported _gmtime32_s missing\n"); return 1; } CHECK(tc_iph_install()); TC_RUN(null_tm); TC_RUN(null_time); TC_RUN(below_min); TC_RUN(accepted_minus_one); TC_RUN(accepted_min); return tc_end(); }
#else
TC_CASE(minus_one) { struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t = -1; errno = 0; r = tc_p__gmtime32_s(&tm, (const __time32_t *)&v_t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
int main(void) { tc_begin(); TC_LOAD_PROC(_gmtime32_s); if (!tc_p__gmtime32_s) { tc_write("TC FAIL " TC_TEST_ID "::exported _gmtime32_s missing\n"); return 1; } CHECK(tc_iph_install()); TC_RUN(null_tm); TC_RUN(null_time); TC_RUN(minus_one); return tc_end(); }
#endif
