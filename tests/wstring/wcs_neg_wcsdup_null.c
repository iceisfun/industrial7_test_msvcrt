/* TC
 * id:    wcs_neg_wcsdup_null
 * area:  wstring
 * funcs: _wcsdup
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _wcsdup
 * note:  PROBE: _wcsdup(NULL) returns NULL (most likely). Whether it also sets errno=EINVAL and calls the
 *        invalid-parameter handler is probed in a separate case (believed from CRT source, not documented).
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

TC_CASE(returns_null)
{
    wchar_t *volatile s = NULL; wchar_t *r;
    tc_iph_install();
    r = _wcsdup(s);
    CHECK(r == NULL);
}
TC_CASE(errno_and_handler)
{
    wchar_t *volatile s = NULL; wchar_t *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _wcsdup(s);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(returns_null); TC_RUN(errno_and_handler); return tc_end(); }
