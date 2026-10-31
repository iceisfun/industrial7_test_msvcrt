/* TC
 * id:    err_underscore_strerror_s_basic
 * area:  errno_param
 * funcs: _strerror_s strerror
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _strerror_s, _strerror (message is newline-terminated)
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(with_prefix)
{
    char buf[128]; errno_t r; char want[160];
    volatile const char *pre = "abc";
    errno = EINVAL;
    r = _strerror_s(buf, sizeof buf, (const char *)pre);
    CHECK_EQ_I(r, 0);
    strcpy(want, "abc: "); strcat(want, strerror(EINVAL)); strcat(want, "\n");
    CHECK_STR(buf, want);
}
TC_CASE(null_prefix)
{
    char buf[128]; errno_t r; char want[160];
    errno = ENOENT;
    r = _strerror_s(buf, sizeof buf, NULL);
    CHECK_EQ_I(r, 0);
    strcpy(want, strerror(ENOENT)); strcat(want, "\n");
    CHECK_STR(buf, want);
}
int main(void)
{
    tc_begin();
    TC_RUN(with_prefix); TC_RUN(null_prefix);
    return tc_end();
}
