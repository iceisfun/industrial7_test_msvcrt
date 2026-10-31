/* TC
 * id:    tm_neg_gmtime32_s_args
 * area:  time
 * funcs:
 * procs: _gmtime32_s
 * crt:   >=90 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gmtime32_s
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
TC_CASE(minus_one) { struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t = -1; errno = 0; r = tc_p__gmtime32_s(&tm, (const __time32_t *)&v_t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
int main(void) { tc_begin(); TC_LOAD_PROC(_gmtime32_s); if (!tc_p__gmtime32_s) { tc_write("TC FAIL " TC_TEST_ID "::exported _gmtime32_s missing\n"); return 1; } CHECK(tc_iph_install()); TC_RUN(null_tm); TC_RUN(null_time); TC_RUN(minus_one); return tc_end(); }
