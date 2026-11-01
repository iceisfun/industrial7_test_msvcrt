/* TC
 * id:    fio_neg_remove_null
 * area:  stdio_file
 * funcs: remove
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs: parameter validation
 * note:  PROBE: remove(NULL) invokes the invalid-parameter handler once, returns -1, errno EINVAL (msvcr80+ validation; remove docs list only EACCES/ENOENT).
 *        OBSERVED (Win10 22H2, class A): remove(NULL) returns -1, errno ENOENT (2), handler NOT called (raw: errno == EINVAL got=2; tc_iph_count == 1 got=0).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
TC_CASE(null_name)
{
    const char * volatile p = NULL;
    int r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = remove(p);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, ENOENT);      /* observed Win10 22H2: remove(NULL) is not validated; plain ENOENT, no handler call */
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(null_name); return tc_end(); }
