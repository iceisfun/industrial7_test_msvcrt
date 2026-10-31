/* TC
 * id:    ucr_new_mode_roundtrip
 * area:  ucrt_only
 * funcs: _set_new_mode _query_new_mode
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_new_mode, _query_new_mode
 */
#include "tc.h"
#include <new.h>

TC_CASE(default_zero) { CHECK_EQ_I(_query_new_mode(), 0); }
TC_CASE(set_get)
{
    CHECK_EQ_I(_set_new_mode(1), 0);
    CHECK_EQ_I(_query_new_mode(), 1);
    CHECK_EQ_I(_set_new_mode(1), 1);
    CHECK_EQ_I(_set_new_mode(0), 1);
    CHECK_EQ_I(_query_new_mode(), 0);
}

int main(void) { tc_begin(); TC_RUN(default_zero); TC_RUN(set_get); return tc_end(); }
