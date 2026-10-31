/* TC
 * id:    tm_neg_asctime_s_small_buffer
 * area:  time
 * funcs: 
 * procs: asctime_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs asctime_s (buffer needs >= 26 chars)
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
TC_CASE(size25) { char b[32]; int r; mk(&t, 117, 0, 15, 0, 0, 0, 0); b[0] = 'x'; tc_iph_count = 0; r = tc_p_asctime_s(b, 25, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); }
TC_CASE(size1) { char b[32]; int r; mk(&t, 117, 0, 15, 0, 0, 0, 0); b[0] = 'x'; tc_iph_count = 0; r = tc_p_asctime_s(b, 1, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); }
TC_CASE(size0) { char b[32]; int r; mk(&t, 117, 0, 15, 0, 0, 0, 0); b[0] = 'x'; tc_iph_count = 0; r = tc_p_asctime_s(b, 0, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size26_ok) { char b[26]; int r; mk(&t, 117, 0, 15, 0, 0, 0, 0); tc_iph_count = 0; r = tc_p_asctime_s(b, 26, &t); CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "Sun Jan 15 00:00:00 2017\n"); }
int main(void) { tc_begin(); tc_iph_install(); TC_LOAD_PROC(asctime_s); CHECK(tc_p_asctime_s); if (!tc_p_asctime_s) return tc_end(); TC_RUN(size25); TC_RUN(size1); TC_RUN(size0); TC_RUN(size26_ok); return tc_end(); }
