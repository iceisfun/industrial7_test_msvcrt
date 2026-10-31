/* TC
 * id:    fio_neg_get_fmode_null
 * area:  stdio_file
 * kind:  negative
 * funcs: 
 * procs: _get_fmode
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _get_fmode (NULL pReturnValue: invalid parameter, EINVAL)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

TC_DECL_PROC(int, _get_fmode, (int *));

TC_CASE(null_ptr)
{
    int r;
    if (!tc_p__get_fmode) TC_SKIP("no _get_fmode");
    tc_iph_count = 0;
    r = tc_p__get_fmode(NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_get_fmode);
    TC_RUN(null_ptr);
    return tc_end();
}
