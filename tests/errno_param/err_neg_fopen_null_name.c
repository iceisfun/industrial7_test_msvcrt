/* TC
 * id:    err_neg_fopen_null_name
 * area:  errno_param
 * funcs: fopen _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fopen, fopen_s: parameter validation
 * note:  Split from the NULL-args test so a crash in one case cannot mask the others.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile nm = "tc_err_neg_fopen_null_name.tmp";

TC_CASE(null_name)
{
    FILE *volatile f; tc_iph_count = 0; errno = 0;
    f = fopen(NULL, "r");
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    if (f) fclose(f);
    DeleteFileA(nm);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(null_name);
    return tc_end();
}
