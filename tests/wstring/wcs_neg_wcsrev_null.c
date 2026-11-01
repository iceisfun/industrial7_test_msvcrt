/* TC
 * id:    wcs_neg_wcsrev_null
 * area:  wstring
 * funcs: _wcsrev
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _strrev, _wcsrev
 * note:  PROBE: _wcsrev(NULL) returns NULL, sets errno EINVAL and invokes the invalid-parameter handler
 *        (parameter validation believed present in VS2005+ CRT; docs silent). Each assertion runs in its own
 *        child process (with a non-fatal handler installed) so an access violation cannot mask the other cases.
 *        Observed Win10 22H2 ucrtbase: _wcsrev(NULL) is NOT validated: it faults with 0xC0000005 even with a non-fatal handler installed,
 *        so the three assertions collapse into the single fault assertion (class C). Stable.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

static int child(const char *n)
{
    wchar_t *volatile s = NULL; wchar_t *r;
    (void)n;
    if (!tc_iph_install()) return 78;
    errno = 0; tc_iph_count = 0;
    r = _wcsrev(s);
    (void)r;
    return 0;   /* only reached when the call did not fault */
}
TC_CASE(null_access_violation)
{
    DWORD code = 99; int ran = tc_spawn_self("nullarg", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);   /* observed: no NULL validation */
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) return child(tc_child_name());
    TC_RUN(null_access_violation);
    return tc_end();
}
