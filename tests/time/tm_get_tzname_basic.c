/* TC
 * id:    tm_get_tzname_basic
 * area:  time
 * funcs: _tzset
 * procs: _get_tzname
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _get_tzname
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

TC_DECL_PROC(int, _get_tzname, (size_t *, char *, size_t, int));

TC_CASE(std_name)
{
    char buf[16]; size_t sz = 0; int r;
    if (!tc_p__get_tzname) TC_SKIP("_get_tzname missing");
    _putenv("TZ=PST8PDT"); _tzset();
    r = tc_p__get_tzname(&sz, buf, sizeof buf, 0);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(sz, 4); CHECK_STR(buf, "PST");
}
TC_CASE(dst_name)
{
    char buf[16]; size_t sz = 0; int r;
    if (!tc_p__get_tzname) TC_SKIP("_get_tzname missing");
    _putenv("TZ=PST8PDT"); _tzset();
    r = tc_p__get_tzname(&sz, buf, sizeof buf, 1);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(sz, 4); CHECK_STR(buf, "PDT");
}
TC_CASE(query_size)
{
    size_t sz = 0; int r;
    if (!tc_p__get_tzname) TC_SKIP("_get_tzname missing");
    _putenv("TZ=PST8PDT"); _tzset();
    r = tc_p__get_tzname(&sz, NULL, 0, 0);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(sz, 4);
}
TC_CASE(exact_fit)
{
    char buf[4]; size_t sz = 0; int r;
    if (!tc_p__get_tzname) TC_SKIP("_get_tzname missing");
    _putenv("TZ=PST8PDT"); _tzset();
    r = tc_p__get_tzname(&sz, buf, 4, 1);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(sz, 4); CHECK_STR(buf, "PDT");
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_tzname);
    TC_RUN(std_name); TC_RUN(dst_name); TC_RUN(query_size); TC_RUN(exact_fit);
    _putenv("TZ="); _tzset();
    return tc_end();
}
