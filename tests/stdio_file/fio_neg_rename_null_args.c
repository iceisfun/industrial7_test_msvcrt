/* TC
 * id:    fio_neg_rename_null_args
 * area:  stdio_file
 * funcs: rename
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs: parameter validation
 * note:  PROBE: rename with NULL oldname or newname invokes the invalid-parameter handler once, returns nonzero, errno EINVAL (msvcr80+ validation; rename docs list EACCES/ENOENT/EINVAL for bad names only).
 *        TRIAGE win10, class A): ucrtbase 10.0.19045 rename(NULL, x) and rename(x, NULL) return nonzero with errno ENOENT (2) and no handler call
 *        (observed got=2, iph 0 for both). Old EINVAL+handler expectation kept for non-UCRT targets (unverified).
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
#if TC_AT_LEAST(140)
    CHECK_EQ_I(errno, ENOENT);             /* observed ucrtbase 10.0.19045: no validation */
    CHECK_EQ_I(tc_iph_count, 0);
#else
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
#endif
}
TC_CASE(new_null)
{
    int r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = rename(nm, nn);
    CHECK(r != 0);
#if TC_AT_LEAST(140)
    CHECK_EQ_I(errno, ENOENT);             /* observed ucrtbase 10.0.19045: no validation */
    CHECK_EQ_I(tc_iph_count, 0);
#else
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
#endif
}
int main(void) { tc_begin(); TC_RUN(old_null); TC_RUN(new_null); return tc_end(); }
