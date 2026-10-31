/* TC
 * id:    tm_wctime64_basic
 * area:  time
 * funcs: _wctime64 _ctime64
 * procs: _wctime64_s
 * crt:   >=90 msvcrt
 * tier:  p1
 * conf:  documented
 * note:  Single-digit days are avoided here because day padding differs between CRT generations; see tm_ctime_probe_day_padding.
 * ref:   MS docs ctime, _wctime64, _wctime64_s
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

TC_DECL_PROC(int, _wctime64_s, (wchar_t *, size_t, const __time64_t *));
static volatile __time64_t vt;

TC_CASE(wide_epoch)
{
    __time64_t t; wchar_t *r; _putenv("TZ=UTC0"); _tzset();
    vt = 864000; t = vt; r = _wctime64(&t);
    CHECK(r != NULL); if (r) CHECK_WSTR(r, L"Sun Jan 11 00:00:00 1970\n");
}
TC_CASE(wide_s)
{
    __time64_t t; wchar_t b[26]; _putenv("TZ=UTC0"); _tzset();
    if (!tc_p__wctime64_s) TC_SKIP("not exported");
    vt = 864000; t = vt;
    CHECK_EQ_I(tc_p__wctime64_s(b, 26, &t), 0);
    CHECK_WSTR(b, L"Sun Jan 11 00:00:00 1970\n");
}
TC_CASE(matches_narrow)
{
    __time64_t t; wchar_t *w; char *n; int i; _putenv("TZ=UTC0"); _tzset();
    vt = 1000259200; t = vt;
    n = _ctime64(&t); CHECK(n != NULL); if (!n) return;
    { static char c[40]; for (i = 0; n[i] && i < 39; i++) c[i] = n[i]; c[i] = 0; n = c; }
    w = _wctime64(&t); CHECK(w != NULL); if (!w) return;
    for (i = 0; n[i]; i++) CHECK_EQ_I(w[i], (unsigned char)n[i]);
    CHECK_EQ_I(w[i], 0);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_wctime64_s); TC_RUN(wide_epoch); TC_RUN(wide_s); TC_RUN(matches_narrow); return tc_end(); }
