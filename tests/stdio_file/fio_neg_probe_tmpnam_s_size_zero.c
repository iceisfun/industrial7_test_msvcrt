/* TC
 * id:    fio_neg_probe_tmpnam_s_size_zero
 * area:  stdio_file
 * kind:  negative
 * funcs:
 * procs: tmpnam_s
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs tmpnam_s
 * note:  PROBE: tmpnam_s(buf, 0) returns EINVAL and invokes the invalid parameter handler (vs ERANGE, no handler).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, tmpnam_s, (char *, size_t));

TC_CASE(size_zero)
{
    char b[8] = "x"; int r, n0 = tc_iph_count;
    if (!tc_p_tmpnam_s) TC_SKIP("tmpnam_s not exported");
    r = tc_p_tmpnam_s(b, 0);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(tmpnam_s); CHECK(tc_p_tmpnam_s != NULL); CHECK(tc_iph_install());
    TC_RUN(size_zero);
    return tc_end();
}
