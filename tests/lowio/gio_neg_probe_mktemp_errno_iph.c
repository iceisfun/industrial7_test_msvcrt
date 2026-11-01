/* TC
 * id:    gio_neg_probe_mktemp_errno_iph
 * area:  lowio
 * funcs: _mktemp _wmktemp
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _mktemp (only promises NULL for a malformed template)
 * note:  PROBE: errno == EINVAL and exactly one invalid-parameter handler call for a malformed non-NULL template;
 *        also that a failed call leaves the template string unchanged (undocumented).
 *        Win10 22H2 19045 (observed): on a malformed template ("abc", "abcXXXXX") _mktemp/_wmktemp return NULL with errno EINVAL, call the handler once, and
 *        SET THE BUFFER TO THE EMPTY STRING (first character zeroed), so the buffer is not left unchanged. The two *_unchanged_on_failure case names are kept.
 *        [unverified] on other CRTs: only ucrtbase 19045 was observed; the expectation is gated with TC_AT_LEAST(140) and the original expectation is kept for older CRTs.
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
    
#if TC_AT_LEAST(140)
CHECK_STR(b, "");     /* observed: the template is truncated to the empty string on failure */
#else
CHECK_STR(b, "abcXXXXX");
#endif

}
TC_CASE(wide_buffer_unchanged_on_failure)
{
    wchar_t b[16] = L"abcXXXXX";
    CHECK(_wmktemp(b) == NULL);
    
#if TC_AT_LEAST(140)
CHECK_WSTR(b, L"");   /* observed */
#else
CHECK_WSTR(b, L"abcXXXXX");
#endif

}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(no_x_errno); TC_RUN(five_x_errno); TC_RUN(iph_called_once); TC_RUN(wide_same);
    TC_RUN(buffer_unchanged_on_failure); TC_RUN(wide_buffer_unchanged_on_failure);
    return tc_end();
}
