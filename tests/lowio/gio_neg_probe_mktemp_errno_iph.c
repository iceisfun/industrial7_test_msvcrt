/* TC
 * id:    gio_neg_probe_mktemp_errno_iph
 * area:  lowio
 * funcs: _mktemp _wmktemp
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mktemp (only promises NULL for a malformed template)
 * note:  PROBE: errno == EINVAL and exactly one invalid-parameter handler call for a malformed non-NULL template;
 *        also that a failed call leaves the template string unchanged (undocumented).
 */
#include "tc.h"
#include <io.h>
#include <errno.h>
#include <wchar.h>

TC_CASE(no_x_errno) { char b[16] = "abc"; errno = 0; CHECK(_mktemp(b) == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(five_x_errno) { char b[16] = "abcXXXXX"; errno = 0; CHECK(_mktemp(b) == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(iph_called_once)
{
    char b[16] = "abc"; int before = tc_iph_count;
    CHECK(_mktemp(b) == NULL);
    CHECK_EQ_I(tc_iph_count, before + 1);
}
TC_CASE(wide_same)
{
    wchar_t b[16] = L"abc"; int before = tc_iph_count;
    errno = 0;
    CHECK(_wmktemp(b) == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, before + 1);
}
TC_CASE(buffer_unchanged_on_failure)
{
    char b[16] = "abcXXXXX";
    CHECK(_mktemp(b) == NULL);
    CHECK_STR(b, "abcXXXXX");
}
TC_CASE(wide_buffer_unchanged_on_failure)
{
    wchar_t b[16] = L"abcXXXXX";
    CHECK(_wmktemp(b) == NULL);
    CHECK_WSTR(b, L"abcXXXXX");
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(no_x_errno); TC_RUN(five_x_errno); TC_RUN(iph_called_once); TC_RUN(wide_same);
    TC_RUN(buffer_unchanged_on_failure); TC_RUN(wide_buffer_unchanged_on_failure);
    return tc_end();
}
