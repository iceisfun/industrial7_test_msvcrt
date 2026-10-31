/* TC
 * id:    fio_neg_set_fmode_invalid
 * area:  stdio_file
 * kind:  negative
 * funcs: 
 * procs: _set_fmode _get_fmode
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_fmode (only _O_TEXT/_O_BINARY/Unicode modes valid; else invalid parameter handler, EINVAL)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

TC_DECL_PROC(int, _set_fmode, (int));
TC_DECL_PROC(int, _get_fmode, (int *));

TC_CASE(bad_value)
{
    int before = -1, after = -2, r;
    if (!tc_p__set_fmode || !tc_p__get_fmode) TC_SKIP("missing procs");
    CHECK_EQ_I(tc_p__get_fmode(&before), 0);
    tc_iph_count = 0; errno = 0;
    r = tc_p__set_fmode(0x1234);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(tc_p__get_fmode(&after), 0); CHECK_EQ_I(after, before);
}
TC_CASE(bad_value_one)
{
    int before = -1, after = -2, r;
    if (!tc_p__set_fmode || !tc_p__get_fmode) TC_SKIP("missing procs");
    CHECK_EQ_I(tc_p__get_fmode(&before), 0);
    tc_iph_count = 0;
    r = tc_p__set_fmode(0x1);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(tc_p__get_fmode(&after), 0); CHECK_EQ_I(after, before);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_set_fmode); TC_LOAD_PROC(_get_fmode);
    TC_RUN(bad_value); TC_RUN(bad_value_one);
    return tc_end();
}
