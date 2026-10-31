/* TC
 * id:    err_neg_strerror_s_unknown_probe
 * area:  errno_param
 * kind:  negative
 * funcs: strerror_s
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs strerror
 * note:  PROBE: unknown errnum text is "Unknown error" and strerror_s returns 0.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(unknown_9999)
{
    char buf[128]; errno_t r; volatile int e = 9999;
    r = strerror_s(buf, sizeof buf, e);
    CHECK_EQ_I(r, 0);
    CHECK_STR(buf, "Unknown error");
}
TC_CASE(unknown_minus1)
{
    char buf[128]; errno_t r; volatile int e = -1;
    r = strerror_s(buf, sizeof buf, e);
    CHECK_EQ_I(r, 0);
    CHECK_STR(buf, "Unknown error");
}
int main(void)
{
    tc_begin();
    TC_RUN(unknown_9999); TC_RUN(unknown_minus1);
    return tc_end();
}
