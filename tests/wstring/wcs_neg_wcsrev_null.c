/* TC
 * id:    wcs_neg_wcsrev_null
 * area:  wstring
 * funcs: _wcsrev
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strrev, _wcsrev
 * note:  PROBE: _wcsrev(NULL) returns NULL, sets errno EINVAL and invokes the invalid-parameter handler
 *        (parameter validation believed present in VS2005+ CRT; docs silent). Each assertion runs in its own
 *        child process (with a non-fatal handler installed) so an access violation cannot mask the other cases.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

static int child(const char *n)
{
    wchar_t *volatile s = NULL; wchar_t *r;
    if (!tc_iph_install()) return 78;
    errno = 0; tc_iph_count = 0;
    r = _wcsrev(s);
    if (n[0] == 'r') return r == NULL ? 0 : 1;
    if (n[0] == 'e') return errno == EINVAL ? 0 : 2;
    return tc_iph_count == 1 ? 0 : 3;
}
static void run(const char *n) { DWORD code = 99; int ran = tc_spawn_self(n, &code, 20000); CHECK(ran); CHECK_EQ_U(code, 0); }
TC_CASE(returns_null) { run("ret"); }
TC_CASE(errno_einval) { run("err"); }
TC_CASE(iph_invoked)  { run("iph"); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) return child(tc_child_name());
    TC_RUN(returns_null); TC_RUN(errno_einval); TC_RUN(iph_invoked);
    return tc_end();
}
