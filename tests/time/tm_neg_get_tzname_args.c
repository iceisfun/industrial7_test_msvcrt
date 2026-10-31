/* TC
 * id:    tm_neg_get_tzname_args
 * area:  time
 * funcs: _tzset
 * procs: _get_tzname
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_tzname (error conditions table)
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(int, _get_tzname, (size_t *, char *, size_t, int));

static void setup(void) { _putenv("TZ=PST8PDT"); _tzset(); tc_iph_count = 0; errno = 0; }

TC_CASE(bad_index_2)
{
    char buf[16]; size_t sz = 5; int r;
    if (!tc_p__get_tzname) TC_SKIP("missing");
    setup(); r = tc_p__get_tzname(&sz, buf, sizeof buf, 2);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(bad_index_neg)
{
    char buf[16]; size_t sz = 5; int r;
    if (!tc_p__get_tzname) TC_SKIP("missing");
    setup(); r = tc_p__get_tzname(&sz, buf, sizeof buf, -1);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_size)
{
    char buf[16]; int r;
    if (!tc_p__get_tzname) TC_SKIP("missing");
    setup(); r = tc_p__get_tzname(NULL, buf, sizeof buf, 0);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buf_nonzero_size)
{
    size_t sz = 5; int r;
    if (!tc_p__get_tzname) TC_SKIP("missing");
    setup(); r = tc_p__get_tzname(&sz, NULL, 8, 0);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_LOAD_PROC(_get_tzname);
    TC_RUN(bad_index_2); TC_RUN(bad_index_neg); TC_RUN(null_size);
    TC_RUN(null_buf_nonzero_size);
    _putenv("TZ="); _tzset();
    return tc_end();
}
