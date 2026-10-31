/* TC
 * id:    fio_neg_rename_null_args
 * area:  stdio_file
 * funcs: rename
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs: parameter validation
 * note:  PROBE: rename with NULL oldname or newname invokes the invalid-parameter handler once, returns nonzero, errno EINVAL (msvcr80+ validation; rename docs list EACCES/ENOENT/EINVAL for bad names only).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
static const char * volatile nn = NULL;
static const char * volatile nm = "tc_fio_neg_rename_null_args_x.tmp";

TC_CASE(old_null)
{
    int r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = rename(nn, nm);
    CHECK(r != 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(new_null)
{
    int r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = rename(nm, nn);
    CHECK(r != 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(old_null); TC_RUN(new_null); return tc_end(); }
