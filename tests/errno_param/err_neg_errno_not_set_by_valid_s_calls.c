/* TC
 * id:    err_neg_errno_not_set_by_valid_s_calls
 * area:  errno_param
 * funcs: strcpy_s memcpy_s
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.5 (errno never cleared by library functions)
 * note:  PROBE: successful bounds-checked calls neither clear nor change errno.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(strcpy_s_ok)
{
    char d[8]; errno_t r; errno = ERANGE;
    r = strcpy_s(d, sizeof d, "abc");
    CHECK_EQ_I(r, 0); CHECK_STR(d, "abc"); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(memcpy_s_ok)
{
    char d[8]; errno_t r; errno = ERANGE;
    r = memcpy_s(d, sizeof d, "abcd", 4);
    CHECK_EQ_I(r, 0); CHECK_MEM(d, "abcd", 4); CHECK_EQ_I(errno, ERANGE);
}

int main(void)
{
    tc_begin();
    TC_RUN(strcpy_s_ok); TC_RUN(memcpy_s_ok);
    errno = 0;
    return tc_end();
}
