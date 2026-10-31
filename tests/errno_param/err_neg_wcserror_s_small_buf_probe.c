/* TC
 * id:    err_neg_wcserror_s_small_buf_probe
 * area:  errno_param
 * kind:  negative
 * funcs: _wcserror_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _wcserror_s
 * note:  PROBE: a too-small buffer is assumed to yield a nonzero return (ERANGE) and a NUL-terminated buffer (as for strcpy_s-style _s functions).
 */
#include "tc.h"
#include <errno.h>
#include <wchar.h>

TC_CASE(small_buffer)
{
    wchar_t buf[8]; errno_t r; int i, term = 0;
    for (i = 0; i < 8; i++) buf[i] = L'x';
    r = _wcserror_s(buf, 4, ENOENT);
    for (i = 0; i < 4; i++) if (!buf[i]) term = 1;
    CHECK(term);
    CHECK(r == 0 || r == ERANGE || r == EINVAL);
}
TC_CASE(small_buffer_returns_error)
{
    wchar_t buf[8]; errno_t r; int i;
    for (i = 0; i < 8; i++) buf[i] = L'x';
    r = _wcserror_s(buf, 4, ENOENT);
    CHECK(r != 0);   /* guess: ERANGE rather than silent truncation */
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(small_buffer); TC_RUN(small_buffer_returns_error);
    return tc_end();
}
