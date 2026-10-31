/* TC
 * id:    tm_neg_asctime_s_null_args
 * area:  time
 * funcs: 
 * procs: asctime_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs asctime_s error table
 * note: msvcrt.dll excluded: invalid-parameter handler support there is not assured
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int wd)
{
    volatile int *p; t->tm_year = y; t->tm_mon = mo; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = wd; t->tm_yday = 0; t->tm_isdst = 0; (void)p;
}
TC_DECL_PROC(int, asctime_s, (char *, size_t, const struct tm *));
static struct tm t;
TC_CASE(null_buf_26) { int r; mk(&t, 117, 0, 1, 0, 0, 0, 0); tc_iph_count = 0; r = tc_p_asctime_s(NULL, 26, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_buf_0) { int r; mk(&t, 117, 0, 1, 0, 0, 0, 0); tc_iph_count = 0; r = tc_p_asctime_s(NULL, 0, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_tm) { char b[26]; int r; b[0] = 'x'; tc_iph_count = 0; r = tc_p_asctime_s(b, 26, NULL); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); }
int main(void) { tc_begin(); tc_iph_install(); TC_LOAD_PROC(asctime_s); CHECK(tc_p_asctime_s); if (!tc_p_asctime_s) return tc_end(); TC_RUN(null_buf_26); TC_RUN(null_buf_0); TC_RUN(null_tm); return tc_end(); }
