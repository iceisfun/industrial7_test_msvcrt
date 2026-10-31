/* TC
 * id:    tm_ctime32_s_basic
 * area:  time
 * funcs:
 * procs: _ctime32_s
 * crt:   >=90 msvcrt
 * tier:  p1
 * conf:  documented
 * note:  Single-digit days are avoided here because day padding differs between CRT generations; see tm_ctime_probe_day_padding.
 * ref:   MS docs ctime_s, _ctime32_s
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

TC_DECL_PROC(int, _ctime32_s, (char *, size_t, const __time32_t *));
static volatile __time32_t vt;

TC_CASE(epoch)
{
    __time32_t t; char b[26]; _putenv("TZ=UTC0"); _tzset();
    if (!tc_p__ctime32_s) TC_SKIP("not exported");
    vt = 864000; t = vt;
    CHECK_EQ_I(tc_p__ctime32_s(b, 26, &t), 0);
    CHECK_STR(b, "Sun Jan 11 00:00:00 1970\n");
}
TC_CASE(int_max)
{
    __time32_t t; char b[26]; _putenv("TZ=UTC0"); _tzset();
    if (!tc_p__ctime32_s) TC_SKIP("not exported");
    vt = 0x7fffffff; t = vt;
    CHECK_EQ_I(tc_p__ctime32_s(b, 26, &t), 0);
    CHECK_STR(b, "Tue Jan 19 03:14:07 2038\n");
}
int main(void) { tc_begin(); TC_LOAD_PROC(_ctime32_s); TC_RUN(epoch); TC_RUN(int_max); return tc_end(); }
