/* TC
 * id:    tm_difftime32_basic
 * area:  time
 * kind:  positive
 * funcs: _difftime32
 * crt:   >=80
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.27.2.2 difftime; MS docs difftime, _difftime32
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

static volatile __time32_t a, b;
TC_CASE(pos) { a = 100; b = 40; CHECK_BITS_D(_difftime32(a, b), 60.0); }
TC_CASE(neg) { a = 40; b = 100; CHECK_BITS_D(_difftime32(a, b), -60.0); }
TC_CASE(max) { a = INT_MAX; b = 0; CHECK_BITS_D(_difftime32(a, b), 2147483647.0); }
TC_CASE(negmax) { a = 0; b = INT_MAX; CHECK_BITS_D(_difftime32(a, b), -2147483647.0); }
int main(void) { tc_begin(); TC_RUN(pos); TC_RUN(neg); TC_RUN(max); TC_RUN(negmax); return tc_end(); }
