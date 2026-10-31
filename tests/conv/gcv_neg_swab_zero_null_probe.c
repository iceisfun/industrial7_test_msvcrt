/* TC
 * id:    gcv_neg_swab_zero_null_probe
 * area:  conv
 * kind:  negative
 * funcs: _swab _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _swab
 * note:  PROBE: does _swab(NULL, NULL, 0) validate pointers before the count check? Believed yes (iph 1, EINVAL).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(null_zero)
{
    tc_iph_count = 0; errno = 0;
    _swab(NULL, NULL, 0);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null_zero);
    return tc_end();
}
