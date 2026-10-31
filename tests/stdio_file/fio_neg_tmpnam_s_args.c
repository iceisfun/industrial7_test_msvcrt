/* TC
 * id:    fio_neg_tmpnam_s_args
 * area:  stdio_file
 * kind:  negative
 * funcs:
 * procs: tmpnam_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs tmpnam_s: str NULL -> EINVAL
 * note:  Too-small buffer is asserted only as nonzero (ERANGE vs EINVAL not asserted); size 0 is in fio_neg_probe_tmpnam_s_size_zero.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, tmpnam_s, (char *, size_t));

TC_CASE(null_buffer)
{
    int r, n0 = tc_iph_count;
    if (!tc_p_tmpnam_s) TC_SKIP("tmpnam_s not exported");
    r = tc_p_tmpnam_s(NULL, 260);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
}
TC_CASE(too_small)
{
    char b[8] = "x"; int r;
    if (!tc_p_tmpnam_s) TC_SKIP("tmpnam_s not exported");
    r = tc_p_tmpnam_s(b, 2);
    CHECK(r != 0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(tmpnam_s); CHECK(tc_p_tmpnam_s != NULL); CHECK(tc_iph_install());
    TC_RUN(null_buffer); TC_RUN(too_small);
    return tc_end();
}
