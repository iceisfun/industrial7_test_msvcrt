/* TC
 * id:    err_neg_wcserror_s_small_buf_probe
 * area:  errno_param
 * kind:  negative
 * funcs: _wcserror_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _wcserror_s
 * note:  [observed on ucrtbase 19045 only; the same expectation on the legacy CRTs listed in crt: is [unverified]] OBSERVED on Win10 22H2 (19045): _wcserror_s(buf,4,ENOENT) returns 0 and leaves L"No " (3 chars + NUL), i.e. it
 *        truncates silently instead of returning ERANGE (same for strerror_s). The earlier guess r != 0 was wrong. Stable.
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
TC_CASE(small_buffer_truncates_returns_zero)
{
    wchar_t buf[8]; errno_t r; int i;
    for (i = 0; i < 8; i++) buf[i] = L'x';
    r = _wcserror_s(buf, 4, ENOENT);
    CHECK_EQ_I(r, 0);                                      /* observed: silent truncation, returns 0 */
    CHECK(buf[0] == L'N' && buf[1] == L'o' && buf[2] == L' ' && buf[3] == 0);   /* "No such file or directory" cut to 3 chars + NUL */
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(small_buffer); TC_RUN(small_buffer_truncates_returns_zero);
    return tc_end();
}
