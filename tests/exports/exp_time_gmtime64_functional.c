/* TC
 * id:    exp_time_gmtime64_functional
 * area:  exports
 * funcs: 
 * procs: _gmtime64 _time64
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _gmtime64, _time64
 */
#include "tc.h"
#include <time.h>

typedef struct tm *(__cdecl *gm_t)(const __time64_t *);
typedef __time64_t (__cdecl *tm64_t)(__time64_t *);

TC_CASE(epoch_plus_one_day)
{
    gm_t g = (gm_t)tc_crt_proc("_gmtime64");
    volatile __time64_t t = 86400;
    __time64_t tt = t;
    struct tm *r;
    CHECK(g != NULL);
    if (!g) return;
    r = g(&tt);
    CHECK(r != NULL);
    if (!r) return;
    CHECK_EQ_I(r->tm_year, 70); CHECK_EQ_I(r->tm_mon, 0); CHECK_EQ_I(r->tm_mday, 2);
    CHECK_EQ_I(r->tm_hour, 0); CHECK_EQ_I(r->tm_wday, 5 /* Friday */); CHECK_EQ_I(r->tm_yday, 1);
}
TC_CASE(time64_is_sane)
{
    tm64_t f = (tm64_t)tc_crt_proc("_time64");
    __time64_t out = 0, ret;
    CHECK(f != NULL);
    if (!f) return;
    ret = f(&out);
    CHECK(ret == out);
    CHECK(ret > 1600000000LL);   /* after Sep 2020 */
    CHECK(f(NULL) >= ret);
}
int main(void) { tc_begin(); TC_RUN(epoch_plus_one_day); TC_RUN(time64_is_sane); return tc_end(); }
