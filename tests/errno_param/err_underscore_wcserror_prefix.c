/* TC
 * id:    err_underscore_wcserror_prefix
 * area:  errno_param
 * funcs: __wcserror __wcserror_s
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs strerror, _strerror, _wcserror, __wcserror (prefix ": " message)
 * note:  Only the prefix and message text are asserted, not the trailing newline.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

TC_CASE(prefix)
{
    volatile const wchar_t *p = L"abc"; wchar_t *r;
    errno = EINVAL;
    r = __wcserror((const wchar_t *)p);
    CHECK(r != NULL);
    if (r) CHECK(wcsncmp(r, L"abc: Invalid argument", 21) == 0);
}
TC_CASE(buffer)
{
    wchar_t b[128]; volatile const wchar_t *p = L"abc"; int rc;
    errno = EINVAL;
    rc = __wcserror_s(b, 128, (const wchar_t *)p);
    CHECK_EQ_I(rc, 0);
    CHECK(wcsncmp(b, L"abc: Invalid argument", 21) == 0);
}
TC_CASE(null_prefix)
{
    volatile const wchar_t *p = NULL; wchar_t *r;
    errno = EINVAL;
    r = __wcserror((const wchar_t *)p);
    CHECK(r != NULL);
    if (r) CHECK(wcsncmp(r, L"Invalid argument", 16) == 0);
}
int main(void) { tc_begin(); TC_RUN(prefix); TC_RUN(buffer); TC_RUN(null_prefix); return tc_end(); }
