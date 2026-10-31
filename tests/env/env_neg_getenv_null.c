/* TC
 * id:    env_neg_getenv_null
 * area:  env
 * kind:  negative
 * funcs: getenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs getenv (Parameter validation: NULL name -> EINVAL, invalid parameter handler invoked)
 * note:  msvcrt.dll excluded: no parameter validation.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(null_name)
{
    char *r; volatile const char *n = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    r = getenv((const char *)n);
    CHECK(r == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void) { tc_begin(); TC_RUN(null_name); return tc_end(); }
