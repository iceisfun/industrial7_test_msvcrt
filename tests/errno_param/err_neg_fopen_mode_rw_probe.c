/* TC
 * id:    err_neg_fopen_mode_rw_probe
 * area:  errno_param
 * funcs: fopen _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fopen: valid mode strings
 * note:  PROBE: mode "rw" is not a valid mode string (only r/w/a first char, then +, b, t, c, n, S, R, T, D, N, ccs=); fopen returns NULL with errno EINVAL rather than treating 'w' as ignorable.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile nm = "tc_err_neg_fopen_mode_rw_probe.tmp";

TC_CASE(mode_rw)
{
    FILE *volatile f; tc_iph_count = 0; errno = 0;
    f = fopen(nm, "rw");
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL);
    if (f) fclose(f);
    DeleteFileA(nm);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(mode_rw);
    return tc_end();
}
