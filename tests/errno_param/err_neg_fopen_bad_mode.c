/* TC
 * id:    err_neg_fopen_bad_mode
 * area:  errno_param
 * funcs: fopen _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fopen: valid mode strings; Parameter validation
 * note:  mode "rw" is split into err_neg_fopen_mode_rw_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile nm = "tc_err_neg_fopen_bad_mode.tmp";

static void bad(const char *mode)
{
    FILE *volatile f; tc_iph_count = 0; errno = 0;
    f = fopen(nm, mode);
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL);
    if (f) fclose(f);
    DeleteFileA(nm);
}
TC_CASE(mode_z)  { bad("z"); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(mode_z);
    return tc_end();
}
