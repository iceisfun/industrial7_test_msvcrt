/* TC
 * id:    wcs_neg_child_wcscpy_s_fatal
 * area:  wstring
 * funcs:
 * procs: wcscpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs "Parameter validation", wcscpy_s: default invalid-parameter handler terminates the process
 * note:  Only asserts the child terminates abnormally (never reaches 'return 77', code != 0). The exact status is
 *        probed in wcs_neg_child_wcscpy_s_fastfail_probe.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

TC_DECL_PROC(errno_t, wcscpy_s, (wchar_t *, size_t, const wchar_t *));
static wchar_t *volatile vnull = NULL;

TC_CASE(child_does_not_return)
{
    DWORD code = 0; int ran = tc_spawn_self("nulldst", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
}
TC_CASE(child_size0_does_not_return)
{
    DWORD code = 0; int ran = tc_spawn_self("small", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
}
int main(void)
{
    const char *c;
    tc_begin(); TC_LOAD_PROC(wcscpy_s);
    if ((c = tc_child_name()) != NULL) {
        tc_iph_default();   /* mingw startup installs a no-op handler; restore the CRT default (fatal) */
        wchar_t d[2];
        if (!tc_p_wcscpy_s) return 78;
        if (c[0] == 'n') tc_p_wcscpy_s(vnull, 10, L"x");
        else tc_p_wcscpy_s(d, 0, L"x");
        return 77;
    }
    CHECK(tc_p_wcscpy_s != NULL);
    TC_RUN(child_does_not_return); TC_RUN(child_size0_does_not_return);
    return tc_end();
}
