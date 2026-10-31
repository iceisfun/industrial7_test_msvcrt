/* TC
 * id:    tm_neg_probe_w_secure_buf_reset
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _wasctime_s _wctime32_s
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs "Security features in the CRT" (secure functions reset the output buffer on failure)
 * note:  PROBE: after EINVAL, when the buffer is non-NULL and size >= 1, buf[0] == 0 for _wasctime_s (size 25, size 1,
 *        tm_year -1) and _wctime32_s (size 25, negative timer). Not stated in the per-function docs. TZ=UTC0.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
TC_DECL_PROC(int, _wasctime_s, (wchar_t *, size_t, const struct tm *));
TC_DECL_PROC(int, _wctime32_s, (wchar_t *, size_t, const __time32_t *));
static volatile size_t vn;
static volatile __time32_t vt;
static struct tm tm_;
static void good(void) { tm_.tm_year = 117; tm_.tm_mon = 0; tm_.tm_mday = 15; tm_.tm_hour = 0; tm_.tm_min = 0; tm_.tm_sec = 0; tm_.tm_wday = 0; tm_.tm_yday = 0; tm_.tm_isdst = 0; }
TC_CASE(asctime_size25) { wchar_t b[32]; int r; good(); b[0] = L'x'; vn = 25; r = tc_p__wasctime_s(b, vn, &tm_); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(b[0], 0); }
TC_CASE(asctime_size1) { wchar_t b[32]; int r; good(); b[0] = L'x'; vn = 1; r = tc_p__wasctime_s(b, vn, &tm_); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(b[0], 0); }
TC_CASE(asctime_year) { wchar_t b[32]; int r; good(); tm_.tm_year = -1; b[0] = L'x'; vn = 26; r = tc_p__wasctime_s(b, vn, &tm_); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(b[0], 0); }
TC_CASE(ctime_size25) { wchar_t b[40]; __time32_t t; int r; _putenv("TZ=UTC0"); _tzset(); vt = 0; t = vt; b[0] = L'x'; vn = 25; r = tc_p__wctime32_s(b, vn, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(b[0], 0); }
TC_CASE(ctime_negative) { wchar_t b[40]; __time32_t t; int r; _putenv("TZ=UTC0"); _tzset(); vt = -1; t = vt; b[0] = L'x'; vn = 40; r = tc_p__wctime32_s(b, vn, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(b[0], 0); }
int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(_wasctime_s); TC_LOAD_PROC(_wctime32_s);
    CHECK(tc_p__wasctime_s && tc_p__wctime32_s); if (!tc_p__wasctime_s || !tc_p__wctime32_s) return tc_end();
    TC_RUN(asctime_size25); TC_RUN(asctime_size1); TC_RUN(asctime_year); TC_RUN(ctime_size25); TC_RUN(ctime_negative); return tc_end();
}
