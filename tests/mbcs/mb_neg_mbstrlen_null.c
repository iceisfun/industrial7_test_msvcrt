/* TC
 * id:    mb_neg_mbstrlen_null
 * area:  mbcs
 * funcs: _mbstrlen
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbstrlen (invalid parameter handler for NULL)
 * note:  PROBE: the errno value (EINVAL) after _mbstrlen(NULL); docs only say NULL is an invalid parameter. Return value (size_t)-1 is the documented error return.
 */
#include "tc.h"
#include <errno.h>
#include <mbstring.h>
#include <stdlib.h>

TC_CASE(null_arg)
{
    const char *volatile p = NULL;
    size_t r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _mbstrlen(p);
    CHECK_EQ_I((long long)r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}
int main(void) { tc_begin(); TC_RUN(null_arg); return tc_end(); }
