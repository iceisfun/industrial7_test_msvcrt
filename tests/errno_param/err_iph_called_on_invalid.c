/* TC
 * id:    err_iph_called_on_invalid
 * area:  errno_param
 * funcs: strcpy_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs Parameter validation; strcpy_s (EINVAL for NULL dest)
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

TC_CASE(fires_and_returns)
{
    char d[8]; volatile const char *s = "x"; char *volatile nul = NULL;
    int r;
    _invalid_parameter_handler old = _set_invalid_parameter_handler(tc_iph);
    tc_iph_count = 0; errno = 0;
    r = strcpy_s(nul, 10, (const char *)s);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
    r = strcpy_s(d, sizeof d, (const char *)s);   /* execution continued; valid call */
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_STR(d, "x");
    _set_invalid_parameter_handler(old);
}
int main(void) { tc_begin(); TC_RUN(fires_and_returns); return tc_end(); }
