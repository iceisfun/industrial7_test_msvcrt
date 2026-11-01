/* TC
 * id:    tm_neg_asctime_probe_year_max
 * area:  time
 * funcs: asctime
 * procs: asctime_s
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs asctime_s
 * note:  PROBE: tm_year 8100 (year 10000, would need a 5-digit year) is rejected: asctime returns NULL
 *        with EINVAL, asctime_s returns EINVAL, handler invoked.
 *        Win10 19045 ucrtbase does NOT reject tm_year 8100 (year 10000): asctime returns "Sun Jan  1 00:00:00 :000\n" and asctime_s returns 0, no errno,
 *        no handler (deviates from Annex K asctime_s, C11 K.3.8.2.1 which requires year <= 9999). Negative year (tm_year -1900) is rejected EINVAL + handler. Older CRTs unverified.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
TC_DECL_PROC(int, asctime_s, (char *, size_t, const struct tm *));
static struct tm t;
static void mk(int y)
{ t.tm_year = y; t.tm_mon = 0; t.tm_mday = 1; t.tm_hour = 0; t.tm_min = 0; t.tm_sec = 0; t.tm_wday = 0; t.tm_yday = 0; t.tm_isdst = 0; }
/* Observed on Win10 ucrtbase: no upper year limit is enforced; tm_year 8100 formats the year digits as ":000" (the 4-digit formatter
 * keeps going past '9'), no errno, no handler. Year 9999 (tm_year 8099) is a normal string. A negative resulting year (tm_year -1900) is rejected. */
TC_CASE(narrow) { char *r; mk(8100); errno = 0; tc_iph_count = 0; r = asctime(&t);
#if TC_CRT_VER >= 140
    CHECK(r != NULL); if (r) CHECK_STR(r, "Sun Jan  1 00:00:00 :000\n"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
#else /* original probe expectation, unverified on older CRTs */
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
#endif
}
TC_CASE(secure) { char b[26]; int r; mk(8100); tc_iph_count = 0; r = tc_p_asctime_s(b, 26, &t);
#if TC_CRT_VER >= 140
    CHECK_EQ_I(r, 0); CHECK_STR(b, "Sun Jan  1 00:00:00 :000\n"); CHECK_EQ_I(tc_iph_count, 0);
#else
    CHECK_EQ_I(r, EINVAL); CHECK(tc_iph_count >= 1);
#endif
}
TC_CASE(year_9999) { char *r; mk(8099); r = asctime(&t); CHECK(r != NULL); if (r) CHECK_STR(r, "Sun Jan  1 00:00:00 9999\n"); }
TC_CASE(negative_year) { char b[26]; int r; mk(-1900); errno = 0; tc_iph_count = 0; r = tc_p_asctime_s(b, 26, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
int main(void) { tc_begin(); tc_iph_install(); TC_LOAD_PROC(asctime_s); TC_RUN(narrow); TC_RUN(year_9999); if (tc_p_asctime_s) { TC_RUN(secure); TC_RUN(negative_year); } return tc_end(); }
