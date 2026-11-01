/* TC
 * id:    tm_neg_ctime64_s_args
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _ctime64_s
 * crt:   >=90 msvcrt
 * tier:  p0
 * conf:  observed
 * ref:   MS docs ctime_s, _ctime64_s error conditions table (EINVAL for NULL/too small buffer, NULL or out-of-range time)
 * note:  beyond_max uses 32536800000 (first rejected value on ucrtbase: EINVAL, errno 22, handler not called); 32535244800 is accepted there.
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
#include <stdlib.h>
#include <limits.h>

TC_DECL_PROC(int, _ctime64_s, (char *, size_t, const __time64_t *));
static volatile __time64_t vt;
static __time64_t zero;

#define NEED() do { if (!tc_p__ctime64_s) TC_SKIP("not exported"); if (!tc_iph_install()) TC_SKIP("no iph"); \
    _putenv("TZ=UTC0"); _tzset(); tc_iph_count = 0; } while (0)

TC_CASE(null_buf) { int r; NEED(); zero = 0; r = tc_p__ctime64_s(NULL, 26, &zero); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size_25)  { char b[40]; int r; NEED(); zero = 0; r = tc_p__ctime64_s(b, 25, &zero); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size_0)   { char b[40]; int r; NEED(); zero = 0; r = tc_p__ctime64_s(b, 0, &zero); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_timer) { char b[26]; int r; NEED(); r = tc_p__ctime64_s(b, 26, NULL); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(negative)
{
    char b[26]; int r; __time64_t t; NEED();
    vt = -1; t = vt; r = tc_p__ctime64_s(b, 26, &t);
    CHECK_EQ_I(r, EINVAL);
}
TC_CASE(beyond_max)
{
    char b[26]; int r; __time64_t t; NEED();
    vt = T64_LOCAL_BAD; t = vt; r = tc_p__ctime64_s(b, 26, &t);
    CHECK_EQ_I(r, EINVAL);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_ctime64_s);
    TC_RUN(null_buf); TC_RUN(size_25); TC_RUN(size_0); TC_RUN(null_timer); TC_RUN(negative); TC_RUN(beyond_max);
    return tc_end();
}
