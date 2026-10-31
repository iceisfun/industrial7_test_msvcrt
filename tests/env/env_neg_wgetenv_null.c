/* TC
 * id:    env_neg_wgetenv_null
 * area:  env
 * kind:  negative
 * funcs: _wgetenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs getenv, _wgetenv (Parameter validation: NULL -> EINVAL, handler invoked)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(null_name)
{
    wchar_t *r; volatile const wchar_t *n = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    r = _wgetenv((const wchar_t *)n);
    CHECK(r == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void) { tc_begin(); TC_RUN(null_name); return tc_end(); }
