/* TC
 * id:    stl_neg_getenv_putenv_invalid
 * area:  stdlib
 * funcs: getenv _putenv
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs getenv, _putenv ("Parameter validation": NULL -> EINVAL, invalid parameter handler)
 * note:  Only NULL arguments are asserted here. See stl_neg_putenv_odd_probe for "noequals", "=value", "".
 */
#include "tc.h"
#include <errno.h>
#include <stdlib.h>

TC_CASE(getenv_null)
{
    const char *r;
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0;
    r = getenv(NULL);
    CHECK(r == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(putenv_null)
{
    int r;
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0;
    r = _putenv(NULL);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(getenv_null); TC_RUN(putenv_null); return tc_end(); }
