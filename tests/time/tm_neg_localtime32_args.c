/* TC
 * id:    tm_neg_localtime32_args
 * area:  time
 * kind:  negative
 * funcs: _localtime32
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _localtime32: NULL on invalid input, errno EINVAL
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

static volatile __time32_t v_t;

TC_CASE(null_timer) { struct tm *p; errno = 0; p = _localtime32(NULL); CHECK(p == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(minus_one) { struct tm *p; v_t = -1; errno = 0; p = _localtime32((const __time32_t *)&v_t); CHECK(p == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(int_min) { struct tm *p; v_t = INT_MIN; p = _localtime32((const __time32_t *)&v_t); CHECK(p == NULL); }

int main(void)
{
    tc_begin();
    tc_iph_install();
    _putenv("TZ=UTC0"); _tzset();
    TC_RUN(null_timer); TC_RUN(minus_one); TC_RUN(int_min);
    _putenv("TZ=");
    return tc_end();
}
