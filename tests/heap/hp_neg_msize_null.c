/* TC
 * id:    hp_neg_msize_null
 * area:  heap
 * funcs: _msize
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _msize: NULL invokes the invalid parameter handler, returns -1, errno EINVAL
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

TC_CASE(null)
{
    void *volatile p = NULL; size_t r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _msize(p);
    CHECK_EQ_U(r, (size_t)-1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(null);
    return tc_end();
}
