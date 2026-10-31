/* TC
 * id:    tm_neg_probe_gmtime64_s_range_handler
 * area:  time
 * funcs:
 * procs: _gmtime64_s
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _gmtime64_s (documents EINVAL, silent on the handler for out-of-range times)
 * note:  PROBE: for an out-of-range time_t (negative, or past the maximum) _gmtime64_s returns EINVAL. We believe the CRT's
 *        range check is a no-exception validation (errno/return only, the invalid-parameter handler is NOT called; this is why
 *        gmtime(-1) returns NULL on Windows instead of terminating the process). Asserts handler count == 0.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static __time64_t g(__time64_t v) { volatile __time64_t x = v; return x; }
TC_DECL_PROC(int, _gmtime64_s, (struct tm *, const __time64_t *));
TC_CASE(negative) { struct tm tm; __time64_t t = g(-1); int r; if (!tc_p__gmtime64_s) TC_SKIP("not exported"); tc_iph_count = 0; r = tc_p__gmtime64_s(&tm, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(beyond_max) { struct tm tm; __time64_t t = g(32535244800LL); int r; if (!tc_p__gmtime64_s) TC_SKIP("not exported"); tc_iph_count = 0; r = tc_p__gmtime64_s(&tm, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(_gmtime64_s); tc_iph_install(); TC_RUN(negative); TC_RUN(beyond_max); return tc_end(); }
