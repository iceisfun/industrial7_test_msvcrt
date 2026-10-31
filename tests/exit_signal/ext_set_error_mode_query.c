/* TC
 * id:    ext_set_error_mode_query
 * area:  exit_signal
 * funcs: _set_error_mode
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_error_mode (modeval 3 = _REPORT_ERRMODE queries; returns old setting)
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(query_does_not_change)
{
    volatile int q = _REPORT_ERRMODE;
    int a = _set_error_mode(q);
    int b = _set_error_mode(q);
    CHECK(a >= 0 && a <= 2);
    CHECK_EQ_I(a, b);
}
TC_CASE(set_returns_old)
{
    volatile int m1 = _OUT_TO_STDERR, q = _REPORT_ERRMODE;
    int old = _set_error_mode(q);
    int r = _set_error_mode(m1);
    CHECK_EQ_I(r, old);
    CHECK_EQ_I(_set_error_mode(q), 1);
    _set_error_mode(old);
    CHECK_EQ_I(_set_error_mode(q), old);
}
TC_CASE(set_msgbox_roundtrip)
{
    volatile int m = _OUT_TO_MSGBOX, q = _REPORT_ERRMODE;
    int old = _set_error_mode(q);
    _set_error_mode(m);
    CHECK_EQ_I(_set_error_mode(q), 2);
    _set_error_mode(old);
}
int main(void)
{
    tc_begin();
    TC_RUN(query_does_not_change); TC_RUN(set_returns_old); TC_RUN(set_msgbox_roundtrip);
    return tc_end();
}
