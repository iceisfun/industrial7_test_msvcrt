/* TC
 * id:    hp_neg_new_mode_invalid
 * area:  heap
 * kind:  negative
 * funcs: _set_new_mode _query_new_mode
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_new_mode: newhandlermode must be 0 or 1; otherwise invalid-parameter handler, returns -1, errno EINVAL
 */
#include "tc.h"
#include <stdlib.h>

#include <errno.h>

static volatile int v_mode;

TC_CASE(mode_two)
{
    int before, r;
    CHECK(tc_iph_install());
    before = _query_new_mode();
    v_mode = 2; errno = 0; tc_iph_count = 0;
    r = _set_new_mode(v_mode);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_query_new_mode(), before);
}
TC_CASE(mode_minus_one)
{
    int before, r;
    CHECK(tc_iph_install());
    before = _query_new_mode();
    v_mode = -1; errno = 0; tc_iph_count = 0;
    r = _set_new_mode(v_mode);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_query_new_mode(), before);
}
TC_CASE(unchanged_after_valid_toggle)
{
    int before = _query_new_mode(), r;
    CHECK(tc_iph_install());
    v_mode = 1; r = _set_new_mode(v_mode); CHECK_EQ_I(r, before); CHECK_EQ_I(_query_new_mode(), 1);
    v_mode = 7; tc_iph_count = 0; r = _set_new_mode(v_mode); CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_query_new_mode(), 1);
    v_mode = before; _set_new_mode(v_mode);
}
int main(void) { tc_begin(); TC_RUN(mode_two); TC_RUN(mode_minus_one); TC_RUN(unchanged_after_valid_toggle); return tc_end(); }
