/* TC
 * id:    io_neg_access_badmode
 * area:  lowio
 * funcs: _access _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _access; Parameter validation
 * note:  observed: _access validates (mode & ~6)==0 -> EINVAL + invalid parameter handler (modes 1,3,8). MS docs list only 00/02/04/06 as valid.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

TC_CASE(mode_1)
{
    volatile int m = 1;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_access(".", m), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(mode_3)
{
    volatile int m = 3;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_access(".", m), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(mode_8)
{
    volatile int m = 8;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_access(".", m), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(mode_1); TC_RUN(mode_3); TC_RUN(mode_8); return tc_end(); }
