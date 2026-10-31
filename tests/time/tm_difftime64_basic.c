/* TC
 * id:    tm_difftime64_basic
 * area:  time
 * kind:  positive
 * funcs: _difftime64
 * crt:   >=80
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.27.2.2 difftime; MS docs difftime, _difftime64
 * note:  msvcrt.dll excluded: _difftime64 export not confirmed for Win7 msvcrt.dll
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

static volatile __time64_t a, b;
TC_CASE(pos) { a = 10; b = 4; CHECK_BITS_D(_difftime64(a, b), 6.0); }
TC_CASE(neg) { a = 4; b = 10; CHECK_BITS_D(_difftime64(a, b), -6.0); }
TC_CASE(zero) { a = 12345; b = 12345; CHECK_BITS_D(_difftime64(a, b), 0.0); }
TC_CASE(big) { a = 1LL << 40; b = 1; CHECK_BITS_D(_difftime64(a, b), 1099511627775.0); }
int main(void) { tc_begin(); TC_RUN(pos); TC_RUN(neg); TC_RUN(zero); TC_RUN(big); return tc_end(); }
