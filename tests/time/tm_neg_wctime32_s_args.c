/* TC
 * id:    tm_neg_wctime32_s_args
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _wctime32_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs ctime_s, _ctime32_s, _wctime32_s (buffer >= 26 elements; negative time invalid; NULL -> EINVAL)
 * note:  msvcrt.dll excluded (no invalid-parameter handler assured). The empty-string-on-failure convention is probed in
 *        tm_neg_probe_w_secure_buf_reset.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
TC_DECL_PROC(int, _wctime32_s, (wchar_t *, size_t, const __time32_t *));
static volatile __time32_t vt;
static volatile size_t vn;
static int call(wchar_t *b, size_t n, const __time32_t *t) { vn = n; tc_iph_count = 0; errno = 0; return tc_p__wctime32_s(b, vn, t); }
TC_CASE(null_buf) { __time32_t t; int r; vt = 0; t = vt; r = call(NULL, 26, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size25) { wchar_t b[40]; __time32_t t; int r; vt = 0; t = vt; b[0] = L'x'; r = call(b, 25, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size0) { wchar_t b[40]; __time32_t t; int r; vt = 0; t = vt; r = call(b, 0, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_timer) { wchar_t b[40]; int r; b[0] = L'x'; r = call(b, 40, NULL); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(negative) { wchar_t b[40]; __time32_t t; int r; _putenv("TZ=UTC0"); _tzset(); vt = -1; t = vt; r = call(b, 40, &t); CHECK_EQ_I(r, EINVAL); }
TC_CASE(valid_26) { wchar_t b[26]; __time32_t t; int r; _putenv("TZ=UTC0"); _tzset(); vt = 0; t = vt; r = call(b, 26, &t); CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(b[24], L'\n'); CHECK_EQ_I(b[25], 0); }
int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(_wctime32_s); CHECK(tc_p__wctime32_s); if (!tc_p__wctime32_s) return tc_end();
    TC_RUN(null_buf); TC_RUN(size25); TC_RUN(size0); TC_RUN(null_timer); TC_RUN(negative); TC_RUN(valid_26); return tc_end();
}
