/* TC
 * id:    env_neg_probe_getenv_missing_errno
 * area:  env
 * kind:  negative
 * funcs: getenv
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs getenv
 * note:  Probes: after getenv of a variable that does not exist, errno is left unchanged (still 0 from a sentinel).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(errno_unchanged_on_missing)
{
    errno = 0;
    CHECK(getenv("TCV_SURELY_MISSING_9876") == NULL);
    CHECK_EQ_I(errno, 0);
}

int main(void) { tc_begin(); TC_RUN(errno_unchanged_on_missing); return tc_end(); }
