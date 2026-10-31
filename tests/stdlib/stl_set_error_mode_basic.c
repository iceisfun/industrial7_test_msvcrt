/* TC
 * id:    stl_set_error_mode_basic
 * area:  stdlib
 * funcs: _set_error_mode
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_error_mode: modeval 0 _OUT_TO_DEFAULT, 1 _OUT_TO_STDERR, 2 _OUT_TO_MSGBOX, 3 _REPORT_ERRMODE; returns old setting
 * note:  The initial value is not asserted here (see stl_set_error_mode_initial_probe); the test restores what it found.
 */
#include "tc.h"
#include <stdlib.h>

static volatile int v1 = 1, v2 = 2, v3 = 3, v0 = 0;

TC_CASE(set_returns_old_and_query)
{
    int orig = _set_error_mode(v3);
    CHECK(orig >= 0 && orig <= 2);
    CHECK_EQ_I(_set_error_mode(v1), orig);   /* returns the previous setting */
    CHECK_EQ_I(_set_error_mode(v3), 1);
    CHECK_EQ_I(_set_error_mode(v2), 1);
    CHECK_EQ_I(_set_error_mode(v3), 2);
    CHECK_EQ_I(_set_error_mode(v0), 2);
    CHECK_EQ_I(_set_error_mode(v3), 0);
    _set_error_mode(orig);
    CHECK_EQ_I(_set_error_mode(v3), orig);
}
int main(void) { tc_begin(); TC_RUN(set_returns_old_and_query); return tc_end(); }
