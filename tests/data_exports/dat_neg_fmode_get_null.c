/* TC
 * id:    dat_neg_fmode_get_null
 * area:  data_exports
 * funcs: _get_fmode
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_fmode: returns EINVAL if pmode is NULL, invoking the invalid parameter handler
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static int r = -1, e = -1;
TC_CASE(null_returns_einval) { CHECK_EQ_I(r, EINVAL); }
TC_CASE(errno_einval)        { CHECK_EQ_I(e, EINVAL); }
TC_CASE(iph_count_1)         { CHECK_EQ_I(tc_iph_count, 1); }

int main(void)
{
    int *volatile p = NULL;
    tc_begin();
    if (!tc_iph_install()) return 1;
    errno = 0;
    r = _get_fmode(p); e = errno;
    TC_RUN(null_returns_einval); TC_RUN(errno_einval); TC_RUN(iph_count_1);
    return tc_end();
}
