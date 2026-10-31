/* TC
 * id:    err_errno_simple_calls_leave_unchanged
 * area:  errno_param
 * funcs: strlen memcpy strcmp abs _errno
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.5p3 (permits, does not require)
 * note:  PROBE: strlen/memcpy/strcmp/abs do not modify errno on Windows CRTs.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>

static volatile int vi = -5;
TC_CASE(keeps_erange)
{
    char a[8], b[8] = "abc"; volatile const char *s = "hello";
    errno = ERANGE;
    CHECK_EQ_U(strlen((const char *)s), 5);
    memcpy(a, b, 4);
    CHECK(strcmp(a, b) == 0);
    CHECK_EQ_I(abs(vi), 5);
    CHECK_EQ_I(errno, ERANGE); errno = 0;
}
TC_CASE(keeps_zero)
{
    char a[8], b[8] = "xy"; volatile const char *s = "hi";
    errno = 0;
    (void)strlen((const char *)s); memcpy(a, b, 3); (void)strcmp(a, b); (void)abs(vi);
    CHECK_EQ_I(errno, 0);
}
int main(void) { tc_begin(); TC_RUN(keeps_erange); TC_RUN(keeps_zero); return tc_end(); }
