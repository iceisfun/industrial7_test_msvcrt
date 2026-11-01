/* TC
 * id:    tm_neg_probe_gmtime_s_error_effects
 * area:  time
 * funcs:
 * procs: _gmtime64_s _gmtime32_s
 * crt:   >=90 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _gmtime64_s (error table); CRT behaviour from memory
 * note:  PROBE: (1) on failure _gmtime64_s/_gmtime32_s set every tm field to -1 (memset 0xff before validation);
 *        (2) a NULL source time invokes the invalid-parameter handler once; (3) a negative or too-large time returns EINVAL
 *        WITHOUT invoking the handler (docs say the handler is invoked; the CRT source is believed to use a no-exception
 *        validation for range checks). All three are unverified against real Windows.
 *        Observed on Win10 19045 ucrtbase: _gmtime*_s accept [-43200, ...]; fill/EINVAL checks use -43201 / 32536850400 / INT_MIN. Handler not invoked for range errors.
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
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

TC_DECL_PROC(int, _gmtime64_s, (struct tm *, const __time64_t *));
TC_DECL_PROC(int, _gmtime32_s, (struct tm *, const __time32_t *));

#if TC_CRT_VER >= 140
#define T32_NEG_BAD (-43201)   /* first rejected value; accepted min is -43200 */
#else
#define T32_NEG_BAD (-1)
#endif
static volatile __time64_t v_t;
static volatile __time32_t v_t32;
static int all_m1(const struct tm *t) { return t->tm_sec == -1 && t->tm_min == -1 && t->tm_hour == -1 && t->tm_mday == -1 && t->tm_mon == -1 && t->tm_year == -1 && t->tm_wday == -1 && t->tm_yday == -1 && t->tm_isdst == -1; }

TC_CASE(fill_null_time64) { struct tm tm; int r; memset(&tm, 0, sizeof tm); r = tc_p__gmtime64_s(&tm, NULL); CHECK_EQ_I(r, EINVAL); CHECK(all_m1(&tm)); }
TC_CASE(fill_negative64) { struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t = T64_GM_NEG_BAD; r = tc_p__gmtime64_s(&tm, (const __time64_t *)&v_t); CHECK_EQ_I(r, EINVAL); CHECK(all_m1(&tm)); }
TC_CASE(fill_beyond64) { struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t = T64_GM_BAD; r = tc_p__gmtime64_s(&tm, (const __time64_t *)&v_t); CHECK_EQ_I(r, EINVAL); CHECK(all_m1(&tm)); }
TC_CASE(fill_negative32) { struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t32 = T32_NEG_BAD; r = tc_p__gmtime32_s(&tm, (const __time32_t *)&v_t32); CHECK_EQ_I(r, EINVAL); CHECK(all_m1(&tm)); }
TC_CASE(fill_intmin32) { struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t32 = INT_MIN; r = tc_p__gmtime32_s(&tm, (const __time32_t *)&v_t32); CHECK_EQ_I(r, EINVAL); CHECK(all_m1(&tm)); }
TC_CASE(no_handler_on_range)
{
    struct tm tm; tc_iph_count = 0;
    v_t = T64_GM_NEG_BAD; tc_p__gmtime64_s(&tm, (const __time64_t *)&v_t);
    v_t = T64_GM_BAD; tc_p__gmtime64_s(&tm, (const __time64_t *)&v_t);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_gmtime64_s); TC_LOAD_PROC(_gmtime32_s); if (!tc_p__gmtime64_s || !tc_p__gmtime32_s) { tc_write("TC FAIL " TC_TEST_ID "::exported _gmtime*_s missing\n"); return 1; } CHECK(tc_iph_install()); TC_RUN(fill_null_time64); TC_RUN(fill_negative64); TC_RUN(fill_beyond64); TC_RUN(fill_negative32); TC_RUN(fill_intmin32); TC_RUN(no_handler_on_range); return tc_end(); }
