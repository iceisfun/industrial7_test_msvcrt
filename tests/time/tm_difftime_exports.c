/* TC
 * id:    tm_difftime_exports
 * area:  time
 * kind:  positive
 * funcs:
 * procs: difftime
 * crt:   msvcr80 msvcrt
 * tier:  p2
 * conf:  standard
 * ref:   C11 7.27.2.2 difftime
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static void settz(const char *s) { _putenv(s); _tzset(); }
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int dst)
{
    volatile int v = y; t->tm_year = v - 1900; t->tm_mon = mo - 1; t->tm_mday = d;
    t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_isdst = dst; t->tm_wday = 0; t->tm_yday = 0;
}

typedef double (__cdecl *difftime_t)(long long, long long);
TC_CASE(exported_works)
{
    difftime_t f = (difftime_t)tc_crt_proc("difftime");
    if (!TC_X64) TC_SKIP("time_t width differs on x86 msvcrt");
    if (!f) TC_SKIP("difftime not exported");
    CHECK_BITS_D(f(10, 4), 6.0);
    CHECK_BITS_D(f(0, 0), 0.0);
}
int main(void) { tc_begin(); TC_RUN(exported_works); return tc_end(); }
