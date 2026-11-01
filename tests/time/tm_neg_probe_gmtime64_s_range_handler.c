/* TC
 * id:    tm_neg_probe_gmtime64_s_range_handler
 * area:  time
 * funcs:
 * procs: _gmtime64_s
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _gmtime64_s (documents EINVAL, silent on the handler for out-of-range times)
 * note:  PROBE: for an out-of-range time_t (negative, or past the maximum) _gmtime64_s returns EINVAL. We believe the CRT's
 *        range check is a no-exception validation (errno/return only, the invalid-parameter handler is NOT called; this is why
 *        gmtime(-1) returns NULL on Windows instead of terminating the process). Asserts handler count == 0.
 *        Observed on Win10 19045 ucrtbase: out-of-range gives EINVAL without calling the handler (iph=0). Range is [-43200, 32536850399], so the probe values are -43201 and 32536850400.
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
static __time64_t g(__time64_t v) { volatile __time64_t x = v; return x; }
TC_DECL_PROC(int, _gmtime64_s, (struct tm *, const __time64_t *));
TC_CASE(negative) { struct tm tm; __time64_t t = g(T64_GM_NEG_BAD); int r; if (!tc_p__gmtime64_s) TC_SKIP("not exported"); tc_iph_count = 0; r = tc_p__gmtime64_s(&tm, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(beyond_max) { struct tm tm; __time64_t t = g(T64_GM_BAD); int r; if (!tc_p__gmtime64_s) TC_SKIP("not exported"); tc_iph_count = 0; r = tc_p__gmtime64_s(&tm, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(_gmtime64_s); tc_iph_install(); TC_RUN(negative); TC_RUN(beyond_max); return tc_end(); }
