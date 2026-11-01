/* TC
 * id:    sec_asctime_s_day_pad_probe
 * area:  secure_crt
 * funcs:
 * procs: asctime_s _gmtime64_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs asctime_s: 26-character fixed-format string "Www Mmm dd hh:mm:ss yyyy\n"; buffer must hold at least 26 chars
 * note:  OBSERVED on Win10 22H2 (19045): asctime_s pads a single-digit day with a SPACE ("Thu Jan  1 00:00:00 1970\n"; day 11
 *        prints "Jan 11"), matching ISO C asctime, not the zero-padded form in the MS docs example. Deterministic (gmtime-based
 *        input, no time zone).
 */
#include "tc.h"
#include <time.h>

typedef int (__cdecl *asctime_s_t)(char *, size_t, const struct tm *);
typedef int (__cdecl *gm_t)(struct tm *, const __int64 *);
static asctime_s_t p_asc; static gm_t p_gm;

TC_CASE(epoch_Thu_Jan_01_1970)
{
    struct tm t; char b[26]; volatile __int64 v = 0;
    CHECK_EQ_I(p_gm(&t, (const __int64 *)&v), 0);
    CHECK_EQ_I(p_asc(b, 26, &t), 0);
    CHECK_STR(b, "Thu Jan  1 00:00:00 1970\n");   /* observed: space padded, as ISO C 7.27.3.1 (the MS docs example text is not what the ucrt prints) */
}
int main(void)
{
    tc_begin();
    p_asc = (asctime_s_t)tc_crt_proc("asctime_s"); p_gm = (gm_t)tc_crt_proc("_gmtime64_s");
    CHECK(p_asc && p_gm); if (!(p_asc && p_gm)) return tc_end();
    TC_RUN(epoch_Thu_Jan_01_1970);
    return tc_end();
}
