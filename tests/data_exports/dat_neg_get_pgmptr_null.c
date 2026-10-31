/* TC
 * id:    dat_neg_get_pgmptr_null
 * area:  data_exports
 * kind:  negative
 * funcs: _get_pgmptr _get_wpgmptr
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _get_pgmptr, _get_wpgmptr: EINVAL if pValue is NULL; invalid parameter handler invoked

 */
#include "tc.h"
#include <errno.h>

#include <stdlib.h>

TC_CASE(get_pgmptr_null_einval) { int r; errno = 0; r = _get_pgmptr(NULL); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(get_pgmptr_null_iph_count_1) { tc_iph_count = 0; _get_pgmptr(NULL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(get_wpgmptr_null_einval) { int r; errno = 0; r = _get_wpgmptr(NULL); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(get_wpgmptr_null_iph_count) { tc_iph_count = 0; _get_wpgmptr(NULL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(returned_value_equals_errno)
{
    int r; errno = 0; r = _get_pgmptr(NULL); CHECK_EQ_I(errno, r);
    errno = 0; r = _get_wpgmptr(NULL); CHECK_EQ_I(errno, r);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(get_pgmptr_null_einval); TC_RUN(get_pgmptr_null_iph_count_1);
    TC_RUN(get_wpgmptr_null_einval); TC_RUN(get_wpgmptr_null_iph_count); TC_RUN(returned_value_equals_errno);
    return tc_end();
}
