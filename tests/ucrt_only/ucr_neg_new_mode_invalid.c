/* TC
 * id:    ucr_neg_new_mode_invalid
 * area:  ucrt_only
 * funcs: _set_new_mode _query_new_mode
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_new_mode (only 0 and 1 valid; EINVAL)
 */
#include "tc.h"
#include <new.h>
#include <errno.h>

TC_CASE(two) { tc_iph_count = 0; errno = 0; CHECK_EQ_I(_set_new_mode(2), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(_query_new_mode(), 0); }
TC_CASE(minus_one) { tc_iph_count = 0; errno = 0; CHECK_EQ_I(_set_new_mode(-1), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(_query_new_mode(), 0); }
TC_CASE(unchanged_when_mode1)
{
    CHECK_EQ_I(_set_new_mode(1), 0);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_set_new_mode(7), -1);
    CHECK_EQ_I(_query_new_mode(), 1);
    _set_new_mode(0);
}

int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(two); TC_RUN(minus_one); TC_RUN(unchanged_when_mode1); return tc_end(); }
