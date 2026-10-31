/* TC
 * id:    env_neg_getenv_missing_no_errno
 * area:  env
 * kind:  negative
 * funcs: getenv
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs getenv
 * note:  An ordinary not-found is not an invalid parameter: NULL is returned, the handler must not run and errno must not
 *        become EINVAL. Whether errno is left untouched is probed in env_neg_probe_getenv_missing_errno.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(missing)
{
    char *r;
    int have = tc_iph_install();
    tc_iph_count = 0; errno = 0;
    r = getenv("TCV_SURELY_MISSING_9876");
    CHECK(r == NULL);                  /* null_return */
    if (have) CHECK_EQ_I(tc_iph_count, 0);
    CHECK(errno != EINVAL);
}

int main(void) { tc_begin(); TC_RUN(missing); return tc_end(); }
