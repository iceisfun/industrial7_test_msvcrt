/* TC
 * id:    exp_neg_gmtime64_s_bad_args
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _gmtime64_s _localtime64_s
 * crt:   >=80
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _gmtime64_s: valid range through 23:59:59 Dec 31 3000 UTC; larger or NULL -> EINVAL
 * note:  NULL arguments -> EINVAL + handler (documented). Far-out-of-range time (2^44 s) -> EINVAL. The documented limit (23:59:59 Dec 31 3000) is not enforced
 *        exactly: Win10 22H2 accepts later times (_gmtime64_s accepts up to 32536850399 = 3001-01-19 21:59:59 UTC and rejects 32536850400 with EINVAL, observed twice; the gmtime limit is time-zone independent), so the old "limit + 1 -> EINVAL" expectation was wrong. tm contents after failure are not asserted.
 *        _localtime64_s's limit depends on the time zone (32536850399 is already rejected in the Pacific reference VM), so only the far-out (2^44) value is asserted for it.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

typedef int (__cdecl *tm_fn)(struct tm *, const __time64_t *);
static tm_fn fns[2];
static const char *names[2] = { "_gmtime64_s", "_localtime64_s" };
static __time64_t gmmax = 32536850399LL, gmmax1 = 32536850400LL, big = 0x100000000000LL, docmax1 = 32535216000LL, okmax = 32535215999LL, zero = 0;

TC_CASE(null_tm_einval) { int i; for (i = 0; i < 2; i++) { CHECK(fns[i] != NULL); if (!fns[i]) continue; tc_iph_count = 0; CHECK_EQ_I(fns[i](NULL, &zero), EINVAL); CHECK_EQ_I(tc_iph_count, 1); } }
TC_CASE(null_time_einval) { struct tm t; int i; for (i = 0; i < 2; i++) { if (!fns[i]) continue; tc_iph_count = 0; CHECK_EQ_I(fns[i](&t, NULL), EINVAL); CHECK_EQ_I(tc_iph_count, 1); } }
TC_CASE(beyond_max_time64_einval) { struct tm t; int i; for (i = 0; i < 2; i++) { if (!fns[i]) continue; CHECK_EQ_I(fns[i](&t, &big), EINVAL); } }
/* observed on Win10 22H2: Dec 31 3000 + 1 s (the documented limit + 1) is still accepted by _gmtime64_s; the real limit is later (3001-01-19T21:59:59Z for gmtime) */
TC_CASE(docs_max_plus_one_still_accepted_gm) { struct tm t; if (!fns[0]) return; CHECK_EQ_I(fns[0](&t, &docmax1), 0); CHECK_EQ_I(t.tm_year, 1101); CHECK_EQ_I(t.tm_mday, 1); }
TC_CASE(gmtime_exact_limit) { struct tm t; if (!fns[0]) return; tc_iph_count = 0; CHECK_EQ_I(fns[0](&t, &gmmax), 0); CHECK_EQ_I(t.tm_year, 1101); CHECK_EQ_I(t.tm_mday, 19); CHECK_EQ_I(t.tm_hour, 21); CHECK_EQ_I(t.tm_sec, 59); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(fns[0](&t, &gmmax1), EINVAL); }
TC_CASE(gmtime_max_ok) { struct tm t; if (!fns[0]) return; CHECK_EQ_I(fns[0](&t, &okmax), 0); CHECK_EQ_I(t.tm_year, 1100); CHECK_EQ_I(t.tm_sec, 59); }

int main(void)
{
    tc_begin(); fns[0] = (tm_fn)tc_crt_proc(names[0]); fns[1] = (tm_fn)tc_crt_proc(names[1]); tc_iph_install();
    TC_RUN(null_tm_einval); TC_RUN(null_time_einval); TC_RUN(beyond_max_time64_einval); TC_RUN(docs_max_plus_one_still_accepted_gm); TC_RUN(gmtime_max_ok); TC_RUN(gmtime_exact_limit);
    return tc_end();
}
