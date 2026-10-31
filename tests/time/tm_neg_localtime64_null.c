/* TC
 * id:    tm_neg_localtime64_null
 * area:  time
 * kind:  negative
 * funcs: _localtime64
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _localtime64: invalid parameter handling (NULL timer -> NULL, errno EINVAL)
 * note:  whether the invalid-parameter handler fires for a NULL timer is isolated in tm_neg_localtime_probe_iph_timer
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

TC_CASE(null_timer)
{
    struct tm *p; tc_iph_install();
    tc_iph_count = 0; errno = 0;
    p = _localtime64(NULL);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, EINVAL);
}
int main(void) { tc_begin(); TC_RUN(null_timer); return tc_end(); }
