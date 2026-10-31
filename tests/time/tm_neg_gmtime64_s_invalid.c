/* TC
 * id:    tm_neg_gmtime64_s_invalid
 * area:  time
 * funcs:
 * procs: _gmtime64_s
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gmtime64_s: returns EINVAL on NULL arguments or time out of range
 * note:  NULL-argument cases assert return, errno and handler count (exactly 1). Out-of-range cases (negative, > max) assert only return and
 *        errno: whether the CRT range check also invokes the invalid-parameter handler is not documented (see tm_neg_probe_gmtime64_s_range_handler).
 *        Only these are asserted; struct contents after failure are probed separately.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>
static __time64_t g(__time64_t v) { volatile __time64_t x = v; return x; }

TC_DECL_PROC(int, _gmtime64_s, (struct tm *, const __time64_t *));
TC_CASE(exported) { CHECK(tc_p__gmtime64_s != NULL); }
TC_CASE(null_tm) { __time64_t t = g(0); int r; if (!tc_p__gmtime64_s) return; tc_iph_count = 0; errno = 0; r = tc_p__gmtime64_s(NULL, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_time) { struct tm tm; int r; if (!tc_p__gmtime64_s) return; tc_iph_count = 0; errno = 0; r = tc_p__gmtime64_s(&tm, NULL); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(negative) { struct tm tm; __time64_t t = g(-1); int r; if (!tc_p__gmtime64_s) return; tc_iph_count = 0; errno = 0; r = tc_p__gmtime64_s(&tm, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(beyond_max) { struct tm tm; __time64_t t = g(32535244800LL); int r; if (!tc_p__gmtime64_s) return; tc_iph_count = 0; errno = 0; r = tc_p__gmtime64_s(&tm, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
int main(void) { tc_begin(); TC_LOAD_PROC(_gmtime64_s); tc_iph_install(); TC_RUN(exported); TC_RUN(null_tm); TC_RUN(null_time); TC_RUN(negative); TC_RUN(beyond_max); return tc_end(); }
