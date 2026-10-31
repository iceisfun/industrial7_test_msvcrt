/* TC
 * id:    tm_neg_wasctime_s_small_buffer
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _wasctime_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs asctime_s, _wasctime_s (buffer must hold at least 26 elements; EINVAL otherwise)
 * note:  msvcrt.dll excluded: invalid-parameter handler support there is not assured. buf[0]==0 on failure follows the
 *        The empty-string-on-failure convention is probed separately in tm_neg_probe_w_secure_buf_reset.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int wd)
{
    t->tm_year = y; t->tm_mon = mo; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = wd; t->tm_yday = 0; t->tm_isdst = 0;
}
TC_DECL_PROC(int, _wasctime_s, (wchar_t *, size_t, const struct tm *));
static struct tm t;
static volatile size_t vsz;
static int call(wchar_t *b, size_t n) { vsz = n; tc_iph_count = 0; return tc_p__wasctime_s(b, vsz, &t); }
TC_CASE(size25) { wchar_t b[32]; int r; mk(&t, 117, 0, 15, 0, 0, 0, 0); b[0] = L'x'; r = call(b, 25); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size1) { wchar_t b[32]; int r; mk(&t, 117, 0, 15, 0, 0, 0, 0); b[0] = L'x'; r = call(b, 1); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size0) { wchar_t b[32]; int r; mk(&t, 117, 0, 15, 0, 0, 0, 0); b[0] = L'x'; r = call(b, 0); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_buf) { int r; mk(&t, 117, 0, 15, 0, 0, 0, 0); r = call(NULL, 26); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size26_ok) { wchar_t b[26]; int r; mk(&t, 117, 0, 15, 0, 0, 0, 0); r = call(b, 26); CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_WSTR(b, L"Sun Jan 15 00:00:00 2017\n"); CHECK_EQ_I(b[24], L'\n'); CHECK_EQ_I(b[25], 0); }
int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(_wasctime_s); CHECK(tc_p__wasctime_s); if (!tc_p__wasctime_s) return tc_end();
    TC_RUN(size25); TC_RUN(size1); TC_RUN(size0); TC_RUN(null_buf); TC_RUN(size26_ok); return tc_end();
}
