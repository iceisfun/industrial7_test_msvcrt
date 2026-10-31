/* TC
 * id:    stl_neg_get_errno_null
 * area:  stdlib
 * funcs: _get_errno
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_errno, _set_errno ("Parameter validation": returns EINVAL if the pointer is NULL)
 * note:  _set_errno(-1) is deliberately not asserted. Positive round trip only uses a valid value.
 */
#include "tc.h"
#include <errno.h>
#include <stdlib.h>

TC_CASE(null_ptr)
{
    errno_t r;
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0;
    r = _get_errno(NULL);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(valid_roundtrip)
{
    int v = -1; errno_t r;
    tc_iph_install(); tc_iph_count = 0;
    r = _set_errno(ERANGE); CHECK_EQ_I(r, 0);
    r = _get_errno(&v);     CHECK_EQ_I(r, 0); CHECK_EQ_I(v, ERANGE);
    CHECK_EQ_I(errno, ERANGE);
    _set_errno(0);
    r = _get_errno(&v);     CHECK_EQ_I(r, 0); CHECK_EQ_I(v, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); TC_RUN(null_ptr); TC_RUN(valid_roundtrip); return tc_end();
}
