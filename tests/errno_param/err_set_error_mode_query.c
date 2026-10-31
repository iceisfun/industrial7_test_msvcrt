/* TC
 * id:    err_set_error_mode_query
 * area:  errno_param
 * funcs: _set_error_mode
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _set_error_mode (_REPORT_ERRMODE=3 queries; returns old setting)
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(query_range)
{
    volatile int m = 3; int r = _set_error_mode(m);
    CHECK(r >= 0 && r <= 2);
}
TC_CASE(query_consistent)
{
    volatile int m = 3; int a = _set_error_mode(m), b = _set_error_mode(m);
    CHECK_EQ_I(a, b);
}
TC_CASE(set_and_restore)
{
    volatile int m = 3, one = 1; int orig = _set_error_mode(m), prev, cur;
    prev = _set_error_mode(one);
    CHECK_EQ_I(prev, orig);
    cur = _set_error_mode(m);
    CHECK_EQ_I(cur, 1);
    _set_error_mode(orig);
    CHECK_EQ_I(_set_error_mode(m), orig);
}
int main(void)
{
    tc_begin();
    TC_RUN(query_range); TC_RUN(query_consistent); TC_RUN(set_and_restore);
    return tc_end();
}
