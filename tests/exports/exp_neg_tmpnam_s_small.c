/* TC
 * id:    exp_neg_tmpnam_s_small
 * area:  exports
 * funcs:
 * procs: tmpnam_s
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  documented
 * ref:   MS docs tmpnam_s: EINVAL for NULL buffer, ERANGE when sizeInChars is too small
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *tmpnam_s_t)(char *, size_t);
static tmpnam_s_t p_tmpnam_s;

TC_CASE(null_einval)
{
    int r;
    if (!p_tmpnam_s) TC_SKIP("tmpnam_s not exported");
    tc_iph_install(); tc_iph_count = 0;
    r = p_tmpnam_s(NULL, 32);
    CHECK_EQ_I(r, EINVAL);
    CHECK(tc_iph_count > 0);
}
TC_CASE(size1_erange)
{
    int r; char b[4] = "abc";
    if (!p_tmpnam_s) TC_SKIP("tmpnam_s not exported");
    tc_iph_install(); tc_iph_count = 0;
    r = p_tmpnam_s(b, 1);
    CHECK_EQ_I(r, ERANGE);
}
int main(void)
{
    tc_begin();
    p_tmpnam_s = (tmpnam_s_t)tc_crt_proc("tmpnam_s");
    TC_RUN(null_einval); TC_RUN(size1_erange);
    return tc_end();
}
