/* TC
 * id:    tm_neg_probe_ctime_s_oversized_size
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _ctime64_s _ctime32_s asctime_s
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs ctime_s, asctime_s (sizeInBytes must be >= 26; no upper bound documented)
 * note:  Probes that a huge size argument (SIZE_MAX, INT_MAX) with a real 26-byte buffer is accepted (return 0) and
 *        not rejected as an RSIZE_MAX violation. The functions are expected to write only 26 bytes.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

TC_DECL_PROC(int, _ctime64_s, (char *, size_t, const __time64_t *));
TC_DECL_PROC(int, _ctime32_s, (char *, size_t, const __time32_t *));
TC_DECL_PROC(int, asctime_s, (char *, size_t, const struct tm *));

TC_CASE(ctime64_size_max) { char b[26]; __time64_t t = 777600; if (!tc_p__ctime64_s) TC_SKIP("no export"); CHECK_EQ_I(tc_p__ctime64_s(b, SIZE_MAX, &t), 0); CHECK_STR(b, "Sat Jan 10 00:00:00 1970\n"); }
TC_CASE(ctime32_int_max) { char b[26]; __time32_t t = 777600; if (!tc_p__ctime32_s) TC_SKIP("no export"); CHECK_EQ_I(tc_p__ctime32_s(b, INT_MAX, &t), 0); CHECK_STR(b, "Sat Jan 10 00:00:00 1970\n"); }
TC_CASE(asctime_size_max)
{
    char b[26]; struct tm tm; int (__cdecl *f)(char *, size_t, const struct tm *) = tc_p_asctime_s;
    if (!f) TC_SKIP("no export");
    tm.tm_year = 117; tm.tm_mon = 9; tm.tm_mday = 23; tm.tm_hour = 14; tm.tm_min = 5; tm.tm_sec = 9; tm.tm_wday = 1; tm.tm_yday = 0; tm.tm_isdst = 0;
    CHECK_EQ_I(f(b, SIZE_MAX, &tm), 0);
    CHECK_STR(b, "Mon Oct 23 14:05:09 2017\n");
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_ctime64_s); TC_LOAD_PROC(_ctime32_s); TC_LOAD_PROC(asctime_s);
    _putenv("TZ=UTC0"); _tzset();
    TC_RUN(ctime64_size_max); TC_RUN(ctime32_int_max); TC_RUN(asctime_size_max);
    _putenv("TZ=");
    return tc_end();
}
