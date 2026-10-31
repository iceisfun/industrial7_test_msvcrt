/* TC
 * id:    cv_neg_atof_l_null_probe
 * area:  conv
 * funcs: _atof_l
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs atof, _atof_l
 * note:  PROBE: _atof_l(NULL, NULL) -> 0.0, errno EINVAL, one invalid-parameter handler call (believed from CRT source validation; undocumented in the _l variant)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static char *volatile v_null;

TC_CASE(atof_l_null)
{
    tc_iph_count = 0; errno = 0;
    CHECK(_atof_l(v_null, NULL) == 0.0);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(atof_l_null); return tc_end(); }
