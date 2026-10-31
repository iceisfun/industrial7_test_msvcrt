/* TC
 * id:    stl_neg_swab_odd_and_invalid
 * area:  stdlib
 * kind:  negative
 * funcs: _swab
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _swab (parameter validation: NULL src/dest -> EINVAL + invalid parameter handler)
 * note:  Only the NULL-pointer validation is asserted here. Odd-n and negative-n behaviour live in stl_neg_swab_negative_n_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

TC_CASE(null_src)
{
    char d[4] = { 'x', 'x', 'x', 'x' };
    tc_iph_count = 0; errno = 0;
    _swab(NULL, d, 4);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_MEM(d, "xxxx", 4);
}
TC_CASE(null_dest)
{
    char s[5]; strcpy(s, "abcd");
    tc_iph_count = 0; errno = 0;
    _swab(s, NULL, 4);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_MEM(s, "abcd", 5);
}
TC_CASE(valid_no_error)
{
    char d[4] = { 'x', 'x', 'x', 'x' };
    tc_iph_count = 0; errno = 0;
    _swab("abcd", d, 4);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
    CHECK_MEM(d, "badc", 4);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null_src); TC_RUN(null_dest); TC_RUN(valid_no_error);
    return tc_end();
}
