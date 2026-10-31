/* TC
 * id:    tm_ctime64_s_basic
 * area:  time
 * funcs: _ctime64
 * procs: _ctime64_s
 * crt:   >=90 msvcrt
 * tier:  p0
 * conf:  documented
 * note:  Single-digit days are avoided here because day padding differs between CRT generations; see tm_ctime_probe_day_padding.
 * ref:   MS docs ctime_s, _ctime32_s, _ctime64_s (buffer of at least 26 bytes)
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

TC_DECL_PROC(int, _ctime64_s, (char *, size_t, const __time64_t *));
static volatile __time64_t vt;

TC_CASE(ret0_and_text)
{
    __time64_t t; char b[26]; _putenv("TZ=UTC0"); _tzset();
    if (!tc_p__ctime64_s) TC_SKIP("not exported");
    vt = 951782400; t = vt;
    CHECK_EQ_I(tc_p__ctime64_s(b, 26, &t), 0);
    CHECK_STR(b, "Tue Feb 29 00:00:00 2000\n");
}
TC_CASE(matches_ctime64)
{
    __time64_t t; char b[26]; char *r; _putenv("TZ=UTC0"); _tzset();
    if (!tc_p__ctime64_s) TC_SKIP("not exported");
    vt = 1000259200; t = vt;
    CHECK_EQ_I(tc_p__ctime64_s(b, 26, &t), 0);
    r = _ctime64(&t); CHECK(r != NULL); if (r) CHECK_STR(b, r);
}
TC_CASE(jst)
{
    __time64_t t; char b[26]; _putenv("TZ=JST-9"); _tzset();
    if (!tc_p__ctime64_s) TC_SKIP("not exported");
    vt = 86400 * 10; t = vt;
    CHECK_EQ_I(tc_p__ctime64_s(b, 26, &t), 0);
    CHECK_STR(b, "Sun Jan 11 09:00:00 1970\n");
    _putenv("TZ=UTC0"); _tzset();
}
TC_CASE(max)
{
    __time64_t t; char b[26]; _putenv("TZ=UTC0"); _tzset();
    if (!tc_p__ctime64_s) TC_SKIP("not exported");
    vt = 32535215999LL; t = vt;
    CHECK_EQ_I(tc_p__ctime64_s(b, 26, &t), 0);
    CHECK_STR(b, "Wed Dec 31 23:59:59 3000\n");
}
int main(void) { tc_begin(); TC_LOAD_PROC(_ctime64_s); TC_RUN(ret0_and_text); TC_RUN(matches_ctime64); TC_RUN(jst); TC_RUN(max); return tc_end(); }
