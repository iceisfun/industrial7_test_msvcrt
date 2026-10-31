/* TC
 * id:    cv_neg_strtol_bad_base_endptr_probe
 * area:  conv
 * funcs: strtol _strtoi64
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtol (endptr / invalid base)
 * note:  PROBE: whether the UCRT stores nptr into *endptr before rejecting an invalid base (we believe it does,
 *        as it initialises endptr first). Older CRTs are not asserted. Documented parts are in cv_neg_strtol_bad_base.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile p = "123";

TC_CASE(strtol_endptr) { char *e = (char *)1; errno = 0; CHECK_EQ_I(strtol(p, &e, 99), 0); CHECK_EQ_P(e, p); }
TC_CASE(i64_endptr) { char *e = (char *)1; errno = 0; CHECK_EQ_I(_strtoi64(p, &e, 1), 0); CHECK_EQ_P(e, p); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(strtol_endptr); TC_RUN(i64_endptr);
    return tc_end();
}
