/* TC
 * id:    sec_neg_asctime_s_args
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: asctime_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs asctime_s: EINVAL if buffer NULL, size < 26, tm NULL, or tm members out of range (e.g. tm_mon > 11)
 */
#include "tc.h"
#include <time.h>

typedef int (__cdecl *asctime_s_t)(char *, size_t, const struct tm *);
static asctime_s_t p_asc;
static struct tm good = { 0, 0, 0, 29, 1, 100, 2, 59, 0 };

TC_CASE(buffer_25_too_small) { char b[64]; int r; tc_iph_count = 0; r = p_asc(b, 25, &good); CHECK_EQ_I(r, 22); CHECK(tc_iph_count >= 1); }
TC_CASE(size_zero) { char b[64]; int r; tc_iph_count = 0; r = p_asc(b, 0, &good); CHECK_EQ_I(r, 22); CHECK(tc_iph_count >= 1); }
TC_CASE(null_buffer) { int r; tc_iph_count = 0; r = p_asc(NULL, 26, &good); CHECK_EQ_I(r, 22); CHECK(tc_iph_count >= 1); }
TC_CASE(null_tm) { char b[64]; int r; tc_iph_count = 0; r = p_asc(b, sizeof b, NULL); CHECK_EQ_I(r, 22); CHECK(tc_iph_count >= 1); }
TC_CASE(month_out_of_range)
{
    char b[64]; struct tm t = good; int r;
    t.tm_mon = 12; r = p_asc(b, sizeof b, &t); CHECK_EQ_I(r, 22);
    t = good; t.tm_hour = 24; r = p_asc(b, sizeof b, &t); CHECK_EQ_I(r, 22);
    t = good; t.tm_wday = 7; r = p_asc(b, sizeof b, &t); CHECK_EQ_I(r, 22);
}
int main(void)
{
    tc_begin();
    p_asc = (asctime_s_t)tc_crt_proc("asctime_s");
    CHECK(p_asc); if (!p_asc) return tc_end();
    tc_iph_install();
    TC_RUN(buffer_25_too_small); TC_RUN(size_zero); TC_RUN(null_buffer); TC_RUN(null_tm); TC_RUN(month_out_of_range);
    return tc_end();
}
