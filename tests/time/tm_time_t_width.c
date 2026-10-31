/* TC
 * id:    tm_time_t_width
 * area:  time
 * funcs: _time64
 * procs: _time32 _mktime32
 * crt:   >=80
 * tier:  p2
 * conf:  documented
 * ref:   MS docs time, _time32, _time64 ("time_t is a 64-bit value unless _USE_32BIT_TIME_T")
 * note:  msvcr80+ x64. sizes/time_t_x64 are compile-time header checks; agree/mktime_agree exercise the DLL.
 */
#include "tc.h"
#include <time.h>

TC_DECL_PROC(__time32_t, _time32, (__time32_t *));
TC_DECL_PROC(__time32_t, _mktime32, (struct tm *));

TC_CASE(sizes) { CHECK_EQ_I(sizeof(__time64_t), 8); CHECK_EQ_I(sizeof(__time32_t), 4); }
TC_CASE(time_t_x64) { CHECK_EQ_I(sizeof(time_t), 8); }
TC_CASE(agree)
{
    __time64_t a = _time64(NULL); __time32_t b = tc_p__time32(NULL);
    long long d = a - (long long)b; if (d < 0) d = -d;
    CHECK(d <= 2);
}
TC_CASE(mktime_agree)
{
    struct tm t1, t2; __time32_t a; __time64_t b;
    t1.tm_year = 100; t1.tm_mon = 1; t1.tm_mday = 29; t1.tm_hour = 0; t1.tm_min = 0; t1.tm_sec = 0; t1.tm_wday = 0; t1.tm_yday = 0; t1.tm_isdst = 0;
    t2 = t1;
    a = tc_p__mktime32(&t1); b = _mktime64(&t2);
    CHECK_EQ_I(a, b);
    CHECK_EQ_I(t1.tm_mday, 29);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_time32); TC_LOAD_PROC(_mktime32);
    CHECK(tc_p__time32 && tc_p__mktime32);
    if (!tc_p__time32 || !tc_p__mktime32) return tc_end();
    TC_RUN(sizes); TC_RUN(time_t_x64); TC_RUN(agree); TC_RUN(mktime_agree);
    return tc_end();
}
