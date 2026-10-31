/* TC
 * id:    cv_neg_strtol_l_null
 * area:  conv
 * funcs: _strtol_l _atoi_l _strtod_l
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs strtol/_strtol_l, atoi/_atoi_l, strtod/_strtod_l: NULL string -> invalid parameter handler, errno EINVAL, return 0
 * note:  NULL string handling is documented for strtol/atoi/strtod; passing a NULL locale_t (current locale) is not spelled out in the docs.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static char *volatile v_null;
#define SETUP do { tc_iph_count = 0; errno = 0; } while (0)

TC_CASE(strtol_l) { char *e = (char *)1; SETUP; CHECK_EQ_I(_strtol_l(v_null, &e, 10, NULL), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(atoi_l) { SETUP; CHECK_EQ_I(_atoi_l(v_null, NULL), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(strtod_l) { char *e = (char *)1; SETUP; CHECK(_strtod_l(v_null, &e, NULL) == 0.0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }

int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(strtol_l); TC_RUN(atoi_l); TC_RUN(strtod_l);
    return tc_end();
}
