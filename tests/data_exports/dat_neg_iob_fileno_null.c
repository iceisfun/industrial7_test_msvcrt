/* TC
 * id:    dat_neg_iob_fileno_null
 * area:  data_exports
 * kind:  negative
 * funcs: _fileno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fileno: NULL stream invokes the invalid parameter handler, returns -1, errno = EINVAL
 */
#include "tc.h"
#include <errno.h>
#include <stdio.h>

static int g_ret, g_errno, g_before;

TC_CASE(returns_minus_one) { CHECK_EQ_I(g_ret, -1); }
TC_CASE(errno_einval) { CHECK_EQ_I(g_errno, EINVAL); }
TC_CASE(iph_count_1) { CHECK_EQ_I(tc_iph_count - g_before, 1); }

int main(void)
{
    FILE *volatile p = NULL;
    tc_begin(); tc_iph_install();
    g_before = tc_iph_count; errno = 0;
    g_ret = _fileno((FILE *)p); g_errno = errno;
    TC_RUN(returns_minus_one); TC_RUN(errno_einval); TC_RUN(iph_count_1);
    return tc_end();
}
