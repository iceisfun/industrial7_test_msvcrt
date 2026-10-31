/* TC
 * id:    mb_neg_setmbcp_invalid_errno
 * area:  mbcs
 * funcs: _setmbcp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _setmbcp
 * note:  probe: errno == EINVAL after failed _setmbcp(12345); reasoning only.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

static volatile int v_cp = 12345;
TC_CASE(errno_einval)
{
    int s = _getmbcp();
    errno = 0;
    CHECK_EQ_I(_setmbcp(v_cp), -1);
    CHECK_EQ_I(errno, EINVAL);
    _setmbcp(s);
}

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(errno_einval); return tc_end(); }
