/* TC
 * id:    io_neg_access_s_badmode
 * area:  lowio
 * funcs: _access_s _waccess_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _access_s; Parameter validation
 * note:  observed: _access_s/_waccess_s reject modes with bits outside 06 (1, 7) with EINVAL + invalid parameter handler.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

TC_CASE(mode_1)
{
    volatile int m = 1;
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    CHECK_EQ_I(_access_s(".", m), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0;
    CHECK_EQ_I(_waccess_s(L".", m), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(mode_7)
{
    volatile int m = 7;
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    CHECK_EQ_I(_access_s(".", m), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(mode_1); TC_RUN(mode_7); return tc_end(); }
