/* TC
 * id:    err_underscore_strerror_s_basic
 * area:  errno_param
 * funcs: _strerror_s strerror
 * crt:   >=90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _strerror_s, _strerror (message is newline-terminated)
 * note:  Observed on Win10 22H2 19045 ucrtbase: _strerror_s writes "prefix: message" WITHOUT the trailing newline that the
 *        _strerror docs describe (got "abc: Invalid argument", old want had a trailing LF).
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
    strcpy(want, "abc: "); strcat(want, strerror(EINVAL));
    CHECK_STR(buf, want);
}
TC_CASE(null_prefix)
{
    char buf[128]; errno_t r; char want[160];
    errno = ENOENT;
    r = _strerror_s(buf, sizeof buf, NULL);
    CHECK_EQ_I(r, 0);
    strcpy(want, strerror(ENOENT));
    CHECK_STR(buf, want);
}
int main(void)
{
    tc_begin();
    TC_RUN(with_prefix); TC_RUN(null_prefix);
    return tc_end();
}
