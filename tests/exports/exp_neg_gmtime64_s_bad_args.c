/* TC
 * id:    exp_neg_gmtime64_s_bad_args
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _gmtime64_s _localtime64_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _gmtime64_s: valid range through 23:59:59 Dec 31 3000 UTC; larger or NULL -> EINVAL
 * note:  tm contents after failure are not asserted.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

typedef int (__cdecl *tm_fn)(struct tm *, const __time64_t *);
static tm_fn fns[2];
static const char *names[2] = { "_gmtime64_s", "_localtime64_s" };
static __time64_t big = 0x793406FFFLL + 1, okmax = 32535215999LL, zero = 0;

TC_CASE(null_tm_einval) { int i; for (i = 0; i < 2; i++) { CHECK(fns[i] != NULL); if (!fns[i]) continue; tc_iph_count = 0; CHECK_EQ_I(fns[i](NULL, &zero), EINVAL); CHECK_EQ_I(tc_iph_count, 1); } }
TC_CASE(null_time_einval) { struct tm t; int i; for (i = 0; i < 2; i++) { if (!fns[i]) continue; tc_iph_count = 0; CHECK_EQ_I(fns[i](&t, NULL), EINVAL); CHECK_EQ_I(tc_iph_count, 1); } }
TC_CASE(beyond_max_time64_einval) { struct tm t; int i; for (i = 0; i < 2; i++) { if (!fns[i]) continue; CHECK_EQ_I(fns[i](&t, &big), EINVAL); } }
TC_CASE(gmtime_max_ok) { struct tm t; if (!fns[0]) return; CHECK_EQ_I(fns[0](&t, &okmax), 0); CHECK_EQ_I(t.tm_year, 1100); CHECK_EQ_I(t.tm_sec, 59); }

int main(void)
{
    tc_begin(); fns[0] = (tm_fn)tc_crt_proc(names[0]); fns[1] = (tm_fn)tc_crt_proc(names[1]); tc_iph_install();
    TC_RUN(null_tm_einval); TC_RUN(null_time_einval); TC_RUN(beyond_max_time64_einval); TC_RUN(gmtime_max_ok);
    return tc_end();
}
