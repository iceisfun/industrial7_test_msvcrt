/* TC
 * id:    err_wcserror_s_basic
 * area:  errno_param
 * funcs: _wcserror_s _wcserror
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _wcserror_s
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <wchar.h>

TC_CASE(matches_wcserror)
{
    wchar_t buf[128]; errno_t r; volatile int e = ENOENT;
    r = _wcserror_s(buf, 128, e);
    CHECK_EQ_I(r, 0);
    CHECK_WSTR(buf, _wcserror(e));
}
TC_CASE(exact_fit)
{
    wchar_t buf[128], small[128]; errno_t r; size_t n; volatile int e = EINVAL; int i;
    r = _wcserror_s(buf, 128, e);
    CHECK_EQ_I(r, 0);
    for (n = 0; buf[n]; n++) ;
    for (i = 0; i < 128; i++) small[i] = L'x';
    r = _wcserror_s(small, n + 1, e);
    CHECK_EQ_I(r, 0);
    CHECK_WSTR(small, buf);
    CHECK_EQ_I(small[n + 1], L'x');
}
TC_CASE(errnum_zero)
{
    wchar_t buf[128]; errno_t r; volatile int e = 0;
    r = _wcserror_s(buf, 128, e);
    CHECK_EQ_I(r, 0);
    CHECK(buf[0] != 0);
    CHECK_WSTR(buf, _wcserror(e));
}
int main(void)
{
    tc_begin();
    TC_RUN(matches_wcserror); TC_RUN(exact_fit); TC_RUN(errnum_zero);
    return tc_end();
}
